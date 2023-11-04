#pragma once

template<typename T>
struct always_false : std::false_type {};

template<typename T>
T FromNapi(const Napi::CallbackInfo& v, const Napi::Value& value)
{
    if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
    {
        static std::string str;
        str = value.As<Napi::String>().Utf8Value(); //hell yeah
        return str.c_str();
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        using NT = std::remove_pointer_t<T>;
        return Napi::ObjectWrap<TypeMap_t<NT>>::Unwrap(value.As<Napi::Object>())->m_handle;
    }
    else return value.As<TypeMap_t<T>>();
}

template<typename T>
Napi::Value ToNapi(const Napi::Env& env, const T& value)
{
    if constexpr (std::is_pointer_v<T>)
    {
        using NT = std::remove_pointer_t<T>;
        return TypeMap_t<NT>::GetCtor().New({ Napi::External<TypeMap_t<NT>>::New(env, reinterpret_cast<TypeMap_t<NT>*>(value)) });
    }
    else if constexpr (std::is_same_v<T, t_api_variant>)
    {
        return FromVariant(env, value);
    }
    else if constexpr (std::is_same_v<T, std::unordered_map<std::string, t_api_variant>>)
    {
        const auto object = Napi::Object::New(env);
        for(auto& [key, val] : value)
            object.Set(key, FromVariant(env, val));
        return object;
    }
    else if constexpr (std::is_same_v<T, std::vector<ElemBase*>>)
    {
        const auto object = Napi::Array::New(env, value.size());

        int idx = 0;
        for(ElemBase* val : value)
            object.Set(idx++, ToNapi(env, val));

        return object;
    }
    else if constexpr (std::is_same_v<T, std::vector<std::string>>)
    {
        const auto object = Napi::Array::New(env, value.size());

        int idx = 0;
        for (std::string val : value)
            object.Set(idx++, ToNapi(env, val));

        return object;
    }
    else return Napi::Value::From(env, value);
}

template<typename T, typename Tuple, std::size_t... Is>
Tuple AnyVectorToTuple(const Napi::CallbackInfo& v, std::index_sequence<Is...>)
{
    return std::make_tuple(FromNapi<std::tuple_element_t<Is, Tuple>>(v, v[Is])...);
}

template<typename T> struct FuncTraits;
template<typename Class, typename Return, typename... Args>
struct FuncTraits<Return(Class::*)(Args...)>
{
    using ClassType = Class;
    using TupleType = std::tuple<Args...>;
    using RetType = Return;
};

template<typename T, typename W, bool Emitter>
class AddonWrapper : public Napi::ObjectWrap<T>
{
    typedef Napi::Value(*t_wrapper)(const Napi::CallbackInfo&, void*);
    typedef Napi::Value(*t_getter_wrapper)(const Napi::CallbackInfo&, void*);
    typedef void(*t_setter_wrapper)(const Napi::CallbackInfo&, const Napi::Value&, void*);
    struct AccessorPair { void* m_getter, *m_setter; };

public:
    W* m_handle;
    std::unordered_map<std::string, std::vector<Napi::FunctionReference>> events;

    template<auto Func>
    static Napi::Value MethodWrapper(const Napi::CallbackInfo& info, W* handle)
    {
        using traits = FuncTraits<decltype(Func)>;
        using TupleType = typename traits::TupleType;
        using RetType = typename traits::RetType;

        TupleType args_tuple = AnyVectorToTuple<T, TupleType>(info, std::make_index_sequence<std::tuple_size_v<TupleType>>{});

        if constexpr (std::is_same_v<RetType, void>)
        {
            std::apply(std::bind_front(Func, handle), args_tuple);
            return Napi::Value();
        }
        else
        {
            RetType ret = std::apply(std::bind_front(Func, handle), args_tuple);
            return ToNapi<RetType>(info.Env(), ret);
        }
    }

    static void Free()
    {
        GetCtor().Reset();
    }

    static Napi::FunctionReference& GetCtor()
    {
        static Napi::FunctionReference ctor_ref;
        return ctor_ref;
    }

    static std::vector<Napi::ClassPropertyDescriptor<T>>& GetPrototype()
    {
        static std::vector<Napi::ClassPropertyDescriptor<T>> m_prototype;
        return m_prototype;
    }

    Napi::Value MethodWrapperCB(const Napi::CallbackInfo& info)
    {
        return reinterpret_cast<t_wrapper>(info.Data())(info, m_handle);
    }

    template<auto Func>
    static Napi::Value GetterWrapper(const Napi::CallbackInfo& info, W* handle)
    {
        using traits = FuncTraits<decltype(Func)>;
        using RetType = typename traits::RetType;

        return ToNapi<RetType>(info.Env(), static_cast<RetType>(std::invoke(std::bind_front(Func, handle))));
    }

    template<auto Func>
    static void SetterWrapper(const Napi::CallbackInfo& info, const Napi::Value& value, W* handle)
    {
        using traits = FuncTraits<decltype(Func)>;
        using TupleType = std::tuple_element_t<0, traits::TupleType>;

        std::invoke(std::bind_front(Func, handle), FromNapi<TupleType>(info, value));
    }

    Napi::Value GetterWrapperCB(const Napi::CallbackInfo& info)
    {
        const auto pair = static_cast<AccessorPair*>(info.Data());
        return reinterpret_cast<t_getter_wrapper>(pair->m_getter)(info, m_handle);
    }

    void SetterWrapperCB(const Napi::CallbackInfo& info, const Napi::Value& value)
    {
        const auto pair = static_cast<AccessorPair*>(info.Data());
        if (!pair->m_setter) return;
        reinterpret_cast<t_setter_wrapper>(pair->m_setter)(info, value, m_handle);
    }

    template<auto G, auto S>
    static void Accessor(const char* name)
    {
        GetPrototype().push_back(
            Napi::InstanceWrap<T>::InstanceAccessor(name, &GetterWrapperCB, &SetterWrapperCB, napi_default, new AccessorPair{ &T::template GetterWrapper<G>, &T::template SetterWrapper<S> })
        );
    }

    template<auto G>
    static void Accessor(const char* name)
    {
        GetPrototype().push_back(
            Napi::InstanceWrap<T>::InstanceAccessor(name, &GetterWrapperCB, &SetterWrapperCB, napi_default, new AccessorPair{ &T::template GetterWrapper<G> })
        );
    }

    template<auto M>
    static void Method(const char* name)
    {
        GetPrototype().push_back(
            Napi::InstanceWrap<T>::InstanceMethod(name, &MethodWrapperCB, napi_default, &T::template MethodWrapper<M>)
        );
    }

    void On(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();
        Napi::String name = info[0].As<Napi::String>();
        Napi::Function handler = info[1].As<Napi::Function>();
        
        events[name].push_back(Napi::Persistent(handler));
    }

    void Emit(const Napi::CallbackInfo& info)
    {
        std::vector<Napi::Value> args;
        args.resize(info.Length() - 1);

        for (size_t i = 1; i < info.Length(); i++)
            args[i - 1] = info[i];

        std::vector<Napi::Function> list;

        for (Napi::FunctionReference& ref : events[info[0].As<Napi::String>()])
            list.push_back(ref.Value());

        for (Napi::Function& func : list)
            func.Call(args);
    }

    void InternalEmit(const std::string& name, const std::vector<t_api_variant>& args = {})
    {
        std::vector<Napi::Function> list;

        for (Napi::FunctionReference& ref : events[name])
            list.push_back(ref.Value());

        std::vector<Napi::Value> n_args;
        n_args.resize(args.size());

        for(int i = 0; i < args.size(); i++)
            n_args[i] = ToNapi(this->Env(), args[i]);

        for (Napi::Function& func : list)
            func.Call(n_args);
    }

    template<typename Te>
    void InternalEmit(const std::string& name, Te* elem, const std::unordered_map<std::string, t_api_variant>& event_data)
    {
        std::vector<Napi::Function> list;

        for (Napi::FunctionReference& ref : events[name])
            list.push_back(ref.Value());

        for (Napi::Function& func : list)
            func.Call({ ToNapi(this->Env(), elem), ToNapi(this->Env(), event_data) });
    }
    
    static void RegisterClass(Napi::Env env, const Napi::Object& exports, const char* name)
    {
        if (Emitter)
        {
            GetPrototype().push_back(T::InstanceMethod("on", &On));
            GetPrototype().push_back(T::InstanceMethod("emit", &Emit));
        }
        GetCtor() = Napi::Persistent(T::DefineClass(env, name, GetPrototype()));
        exports.Set(name, GetCtor().Value());
    }

public:
    AddonWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<T>(info)
    {
        if (info.Length() == 1 && info[0].IsExternal())
        {
            m_handle = info[0].As<Napi::External<W>>().Data();
        }
    }
};
