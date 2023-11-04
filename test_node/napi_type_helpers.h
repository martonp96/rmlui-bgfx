#pragma once
#include <type_traits>

class AppBase;
class ElemBase;
class DocBase;

class AppWrapper;
class ElemWrapper;
class DocWrapper;

template<typename A, typename = void>
struct TypeMap {
    static_assert(!std::is_same<A, A>::value, "Type is not mapped to any type.");
};

template<> struct TypeMap<int> { using type = Napi::Number; };
template<> struct TypeMap<float> { using type = Napi::Number; };
template<> struct TypeMap<double> { using type = Napi::Number; };
template<> struct TypeMap<bool> { using type = Napi::Boolean; };
template<> struct TypeMap<std::string> { using type = Napi::String; };
template<> struct TypeMap<char*> { using type = Napi::String; };
template<> struct TypeMap<const char*> { using type = Napi::String; };

template<> struct TypeMap<AppBase> { using type = AppWrapper; };
template<> struct TypeMap<ElemBase> { using type = ElemWrapper; };
template<> struct TypeMap<DocBase> { using type = DocWrapper; };
template<typename A> using TypeMap_t = typename TypeMap<A>::type;

Napi::Value FromVariant(const Napi::Env& env, const t_api_variant& variant)
{
    switch (variant.index())
    {
    case 0: return Napi::Value::From(env, std::get<bool>(variant));
    case 1: return Napi::Value::From(env, std::get<uint64_t>(variant));
    case 2: return Napi::Value::From(env, std::get<int64_t>(variant));
    case 3: return Napi::Value::From(env, std::get<double>(variant));
    case 4: return Napi::Value::From(env, std::get<std::string>(variant));
    case 5:
    {
        const vec2 vec = std::get<vec2>(variant);
        const auto object = Napi::Object::New(env);
        object.Set("x", vec.x);
        object.Set("y", vec.y);
        return object;
    }
    case 6:
    {
        const vec3 vec = std::get<vec3>(variant);
        const auto object = Napi::Object::New(env);
        object.Set("x", vec.x);
        object.Set("y", vec.y);
        object.Set("z", vec.z);
        return object;
    }
    case 7:
    {
        const vec4 vec = std::get<vec4>(variant);
        const auto object = Napi::Object::New(env);
        object.Set("x", vec.x);
        object.Set("y", vec.y);
        object.Set("z", vec.z);
        object.Set("w", vec.w);
        return object;
    }
    case 8:
    {
        const fcolor vec = std::get<fcolor>(variant);
        const auto object = Napi::Object::New(env);
        object.Set("r", vec.r);
        object.Set("g", vec.g);
        object.Set("b", vec.b);
        object.Set("a", vec.a);
        return object;
    }
    case 9:
    {
        const color vec = std::get<color>(variant);
        const auto object = Napi::Object::New(env);
        object.Set("r", vec.r);
        object.Set("g", vec.g);
        object.Set("b", vec.b);
        object.Set("a", vec.a);
        return object;
    }
    }
    return Napi::Value();
}