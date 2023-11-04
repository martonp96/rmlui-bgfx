#pragma once
#include <variant>
#include <string>

#if !defined(API_FUNC) 
#if defined(BUILD_STATIC) || defined(BUILD_BINARY)
#define API_FUNC
#elif defined(_WIN32)
#if defined(BUILD_SHARED)
#define API_FUNC extern "C" __declspec(dllexport)
#else
#define API_FUNC extern "C" __declspec(dllimport)
#endif
#else
#define API_FUNC
#endif
#endif

class AppBase;
class ElemBase;
class DocBase;

struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };
struct fcolor { float r, g, b, a; };
struct color { unsigned char r, g, b, a; };

typedef std::variant<bool, uint64_t, int64_t, double, std::string, vec2, vec3, vec4, fcolor, color, ElemBase*> t_api_variant;

namespace Core
{
    API_FUNC AppBase* CreateApp(int width, int height);
};

class AppBase
{
public:
    virtual ~AppBase() = default;

    typedef std::function<bool(ElemBase*, std::string, std::unordered_map<std::string, t_api_variant>)> t_rml_event_handler;
    typedef std::function<void()> t_generic_event_handler;

    virtual void Destroy() = 0;
    virtual void Start() = 0; //needed so you can register event handlers before they are fired
    virtual bool IsRunning() = 0;
    virtual bool IsReady() = 0;
    virtual void RunLoop() = 0;

    virtual DocBase* CreateDocument(std::string rml) = 0;
    virtual DocBase* LoadDocument(std::string path) = 0;
    virtual DocBase* GetDocument() = 0;

    virtual void RegisterRmlEventHandler(const t_rml_event_handler& handler) = 0;
    virtual void RegisterRenderEventHandler(const t_generic_event_handler& handler) = 0;
    virtual void RegisterUpdateEventHandler(const t_generic_event_handler& handler) = 0;
    virtual void RegisterWindowInitEventHandler(const t_generic_event_handler& handler) = 0;
    virtual void RegisterRenderInitEventHandler(const t_generic_event_handler& handler) = 0;

    virtual void LoadFontFace(std::string path, bool is_default) = 0;
};

class ElemBase
{
public:
    virtual ~ElemBase() = default;

    virtual bool AddClass(std::string name) = 0;
    virtual bool RemoveClass(std::string name) = 0;
    virtual bool HasClass(std::string name) = 0;
    virtual std::string GetClassList() = 0;

    virtual bool AddPseudoClass(std::string name) = 0;
    virtual bool RemovePseudoClass(std::string name) = 0;
    virtual bool HasPseudoClass(std::string name) = 0;
    virtual std::vector<std::string> GetPseudoClassList() = 0;

    virtual void SetOffset(ElemBase* offset_parent, float offset_x, float offset_y, bool fixed) = 0;
    virtual t_api_variant GetRelativeOffset() = 0;
    virtual t_api_variant GetAbsoluteOffset() = 0;
    virtual float GetBaseline() = 0;
    virtual float GetZIndex() = 0;

    virtual bool IsPointWithinElement(float point_x, float point_y) = 0;

    virtual bool SetProperty(std::string name, std::string value) = 0;
    virtual bool RemoveProperty(std::string name) = 0;
    virtual bool HasProperty(std::string name) = 0;
    virtual bool HasLocalProperty(std::string name) = 0;
    virtual std::string GetProperty(std::string name) = 0;
    virtual std::string GetLocalProperty(std::string name) = 0;
    virtual float GetPropertyAbsoluteValue(std::string name) = 0;

    virtual t_api_variant GetContainingBlock() = 0;
    
    virtual ElemBase* GetFocusedElement() = 0;

    virtual std::string GetTagName() = 0;
    virtual std::string GetId() = 0;
    virtual void SetId(std::string id) = 0;

    virtual void SetAttribute(std::string name, std::string value) = 0;
    virtual bool RemoveAttribute(std::string name) = 0;
    virtual bool HasAttribute(std::string name) = 0;
    virtual std::string GetAttribute(std::string name) = 0;
    virtual std::unordered_map<std::string, t_api_variant> GetAttributes() = 0;

    virtual float GetAbsoluteLeft() = 0;
    virtual float GetAbsoluteTop() = 0;
    virtual float GetClientLeft() = 0;
    virtual float GetClientTop() = 0;
    virtual float GetClientWidth() = 0;
    virtual float GetClientHeight() = 0;
    virtual ElemBase* GetOffsetParent() = 0;
    virtual float GetOffsetLeft() = 0;
    virtual float GetOffsetTop() = 0;
    virtual float GetOffsetWidth() = 0;
    virtual float GetOffsetHeight() = 0;

    virtual float GetScrollLeft() = 0;
    virtual void SetScrollLeft(float value) = 0;
    virtual float GetScrollTop() = 0;
    virtual void SetScrollTop(float value) = 0;
    virtual float GetScrollWidth() = 0;
    virtual float GetScrollHeight() = 0;

    virtual bool IsVisible() = 0;

    virtual ElemBase* GetParent() = 0;
    virtual ElemBase* GetClosest(std::string selectors) = 0;
    virtual ElemBase* GetNextSibling() = 0;
    virtual ElemBase* GetPreviousSibling() = 0;
    virtual ElemBase* GetFirstChild() = 0;
    virtual ElemBase* GetLastChild() = 0;
    virtual ElemBase* GetChild(int index) = 0;
    virtual int GetChildCount() = 0;
    virtual ElemBase* AppendChild(ElemBase* new_element) = 0;
    virtual ElemBase* InsertBefore(ElemBase* new_element, ElemBase* adjacent_element) = 0;
    virtual ElemBase* ReplaceChild(ElemBase* new_element, ElemBase* old_element) = 0;
    virtual ElemBase* RemoveChild(ElemBase* child) = 0;
    virtual bool HasChildren() = 0;

    virtual std::string GetInnerRML() = 0;
    virtual void SetInnerRML(std::string value) = 0;

    virtual bool Focus() = 0;
    virtual void Blur() = 0;
    virtual void Click() = 0;
    virtual void ScrollIntoView(bool align_with_top = true) = 0;

    virtual ElemBase* GetElementById(std::string id) = 0;
    virtual std::vector<ElemBase*> GetElementsByTagName(std::string tag) = 0;
    virtual std::vector<ElemBase*> GetElementsByClassName(std::string tag) = 0;
    virtual ElemBase* QuerySelector(std::string selector) = 0;
    virtual std::vector<ElemBase*> QuerySelectorAll(std::string selector) = 0;

    virtual ElemBase* GetOwnerDocument() = 0;

    virtual std::string FormControlGetValue() = 0;
    virtual void FormControlSetValue(std::string value) = 0;
};

class DocBase : public virtual ElemBase
{
public:
    virtual ~DocBase() = default;

    virtual void SetVisible(bool toggle) = 0;
    virtual void Update() = 0;

    virtual DocBase* GetBody() = 0;
    virtual ElemBase* CreateElement(std::string name) = 0;
    virtual ElemBase* CreateTextNode(std::string text) = 0;
};