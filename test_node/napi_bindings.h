#pragma once

class AppWrapper : public AddonWrapper<AppWrapper, AppBase, true>
{
public:
    std::vector<Napi::FunctionReference*> m_render_init_callbacks;

    static void Init(Napi::Env env, const Napi::Object& exports)
    {
        RegisterPrototypes<AppWrapper, AppBase>();
        RegisterClass(env, exports, "App");
    }

    template<typename B, typename W>
    static void RegisterPrototypes()
    {
        B::Method<&W::Start>("start");
        B::Method<&W::IsRunning>("isRunning");
        B::Method<&W::IsReady>("isReady");
        B::Method<&W::CreateDocument>("createDocument");
        B::Method<&W::LoadDocument>("loadDocument");
        B::Method<&W::GetDocument>("getDocument");
        B::Method<&W::LoadFontFace>("loadFontFace");
    }

    AppWrapper(const Napi::CallbackInfo& info) : AddonWrapper<AppWrapper, AppBase, true>(info)
    {
        if (info.Length() == 2)
        {
            auto width = info[0].ToNumber().Int32Value();
            auto height = info[1].ToNumber().Int32Value();

            m_handle = Core::CreateApp(width, height);

            StartMainLoop(info.Env());
            
            m_handle->RegisterRenderInitEventHandler([this] {
                InternalEmit("init");
            });

            m_handle->RegisterRmlEventHandler([this](ElemBase* elem, std::string name, std::unordered_map<std::string, t_api_variant> event_data) {
                InternalEmit(name, elem, event_data);
                return true;
            });
        }
    }

    static void RunLoop(const Napi::CallbackInfo& info)
    {
        const auto handle = reinterpret_cast<AppBase*>(info.Data());
        if (handle->IsRunning())
            handle->RunLoop();
    }

    //there is really no other way to run an independent loop from main thread?
    void StartMainLoop(Napi::Env env)
    {
        const auto setIntervalFn = env.Global().Get("setInterval");
        const auto func = Napi::Function::New(env, RunLoop, "RunLoop", this->m_handle);
        setIntervalFn.As<Napi::Function>().Call({ func, Napi::Number::New(env, 0) });
    }

    ~AppWrapper() override
    {
        printf("AppWrapper dtor\n");
    }
};

class ElemWrapper : public AddonWrapper<ElemWrapper, ElemBase, true>
{
public:
    static void Init(Napi::Env env, const Napi::Object& exports)
    {
        RegisterPrototypes<ElemWrapper, ElemBase>();
        RegisterClass(env, exports, "Elem");
    }

    template<typename B, typename W>
    static void RegisterPrototypes()
    {
        B::Accessor<&W::GetId, &W::SetId>("id");
        B::Accessor<&W::IsVisible>("visible");

        B::Accessor<&W::GetRelativeOffset>("relativeOffset");
        B::Accessor<&W::GetAbsoluteOffset>("absoluteOffset");
        B::Accessor<&W::GetBaseline>("baseline");
        B::Accessor<&W::GetZIndex>("zIndex");
        B::Accessor<&W::GetContainingBlock>("containingBlock");
        B::Accessor<&W::GetFocusedElement>("focusedElement");
        B::Accessor<&W::GetTagName>("tagName");
        B::Accessor<&W::GetAbsoluteLeft>("absoluteLeft");
        B::Accessor<&W::GetAbsoluteTop>("absoluteTop");
        B::Accessor<&W::GetClientLeft>("clientLeft");
        B::Accessor<&W::GetClientTop>("clientTop");
        B::Accessor<&W::GetClientWidth>("clientWidth");
        B::Accessor<&W::GetClientHeight>("clientHeight");
        B::Accessor<&W::GetOffsetLeft>("offsetLeft");
        B::Accessor<&W::GetOffsetTop>("offsetTop");
        B::Accessor<&W::GetOffsetWidth>("offsetWidth");
        B::Accessor<&W::GetOffsetHeight>("offsetHeight");
        B::Accessor<&W::GetScrollLeft, &W::SetScrollLeft>("scrollLeft");
        B::Accessor<&W::GetScrollTop, &W::SetScrollTop>("scrollTop");
        B::Accessor<&W::GetScrollWidth>("scrollWidth");
        B::Accessor<&W::GetScrollHeight>("scrollHeight");
        B::Accessor<&W::IsVisible>("isVisible");
        B::Accessor<&W::GetParent>("parent");
        B::Accessor<&W::GetNextSibling>("nextSibling");
        B::Accessor<&W::GetPreviousSibling>("previousSibling");
        B::Accessor<&W::GetFirstChild>("firstChild");
        B::Accessor<&W::GetLastChild>("lastChild");
        B::Accessor<&W::GetChildCount>("childCount");
        B::Accessor<&W::GetInnerRML, &W::SetInnerRML>("innerRML");
        B::Accessor<&W::HasChildren>("hasChildren");
        B::Accessor<&W::GetOwnerDocument>("ownerDocument");
        B::Accessor<&W::GetAttributes>("attributes");
        B::Accessor<&W::GetClassList>("classList");
        B::Accessor<&W::GetPseudoClassList>("pseudoClassList");

        B::Method<&W::AddClass>("addClass");
        B::Method<&W::RemoveClass>("removeClass");
        B::Method<&W::HasClass>("hasClass");
        B::Method<&W::AddPseudoClass>("addPseudoClass");
        B::Method<&W::RemovePseudoClass>("removePseudoClass");
        B::Method<&W::HasPseudoClass>("hasPseudoClass");
        B::Method<&W::SetOffset>("setOffset");
        B::Method<&W::IsPointWithinElement>("isPointWithinElement");
        B::Method<&W::SetProperty>("setProperty");
        B::Method<&W::RemoveProperty>("removeProperty");
        B::Method<&W::HasProperty>("hasProperty");
        B::Method<&W::HasLocalProperty>("hasLocalProperty");
        B::Method<&W::GetProperty>("getProperty");
        B::Method<&W::GetLocalProperty>("getLocalProperty");
        B::Method<&W::GetPropertyAbsoluteValue>("getPropertyAbsoluteValue");
        B::Method<&W::SetAttribute>("setAttribute");
        B::Method<&W::RemoveAttribute>("removeAttribute");
        B::Method<&W::HasAttribute>("hasAttribute");
        B::Method<&W::GetAttribute>("getAttribute");
        B::Method<&W::GetClosest>("getClosest");
        B::Method<&W::GetChild>("getChild");
        B::Method<&W::AppendChild>("appendChild");
        B::Method<&W::InsertBefore>("insertBefore");
        B::Method<&W::ReplaceChild>("replaceChild");
        B::Method<&W::RemoveChild>("removeChild");
        B::Method<&W::Focus>("focus");
        B::Method<&W::Blur>("blur");
        B::Method<&W::Click>("click");
        B::Method<&W::ScrollIntoView>("scrollIntoView");
        B::Method<&W::GetElementById>("getElementById");
        B::Method<&W::GetElementsByTagName>("getElementsByTagName");
        B::Method<&W::GetElementsByClassName>("getElementsByClassName");
        B::Method<&W::QuerySelector>("querySelector");
        B::Method<&W::QuerySelectorAll>("querySelectorAll");
        B::Method<&W::GetAttributes>("getAttributes");
    }

    ElemWrapper(const Napi::CallbackInfo& info) : AddonWrapper<ElemWrapper, ElemBase, true>(info){}
    ~ElemWrapper() override
    {
        printf("ElemWrapper dtor\n");
    }
};

class DocWrapper : public AddonWrapper<DocWrapper, DocBase, true>
{
public:
    static void Init(Napi::Env env, const Napi::Object& exports)
    {
        RegisterPrototypes<DocWrapper, DocBase>();
        RegisterClass(env, exports, "Doc");
    }

    template<typename B, typename W>
    static void RegisterPrototypes()
    {
        ElemWrapper::RegisterPrototypes<B, W>();
        B::Method<&W::CreateElement>("createElement");
        B::Method<&W::CreateTextNode>("createTextNode");
        B::Method<&W::SetVisible>("setVisible");

        //B::Accessor<&W::GetTitle, &W::SetTitle>("title");
        //B::Accessor<&W::GetSourceUrl>("sourceUrl");
        //B::Accessor<&W::IsModal>("isModal");
        B::Accessor<&W::GetBody>("body");
        B::Method<&W::Update>("update");
    }

    DocWrapper(const Napi::CallbackInfo& info) : AddonWrapper<DocWrapper, DocBase, true>(info) {}

    ~DocWrapper() override
    {
        printf("DocWrapper dtor\n");
    }
};
