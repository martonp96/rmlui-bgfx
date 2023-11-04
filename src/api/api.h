#pragma once
#include "api_base.h"

namespace window
{
    class CWindow;
}

class Doc;
class Elem;

class App : public AppBase
{
    window::CWindow* m_wnd;
public:
    App(int width, int height);

    void Destroy() override;
    void Start() override; //needed so you can register event handlers before they are fired
    bool IsRunning() override;
    bool IsReady() override;
    void RunLoop() override;

    DocBase* CreateDocument(std::string rml) override;
    DocBase* LoadDocument(std::string path) override;
    DocBase* GetDocument() override;

    void RegisterRmlEventHandler(const t_rml_event_handler& handler) override;
    void RegisterRenderEventHandler(const t_generic_event_handler& handler) override;
    void RegisterUpdateEventHandler(const t_generic_event_handler& handler) override;
    void RegisterWindowInitEventHandler(const t_generic_event_handler& handler) override;
    void RegisterRenderInitEventHandler(const t_generic_event_handler& handler) override;

    void LoadFontFace(std::string path, bool is_default) override;
};

class Elem : public virtual ElemBase
{
protected:
    Rml::Element* element = nullptr;
    //Rml::ElementPtr* element_ptr = nullptr;

public:
    Elem(Rml::Element* elem);
    //Elem(Rml::ElementPtr* elem);

    bool AddClass(std::string name) override;
    bool RemoveClass(std::string name) override;
    bool HasClass(std::string name) override;
    std::string GetClassList() override;

    bool AddPseudoClass(std::string name) override;
    bool RemovePseudoClass(std::string name) override;
    bool HasPseudoClass(std::string name) override;
    std::vector<std::string> GetPseudoClassList() override;

    void SetOffset(ElemBase* offset_parent, float offset_x, float offset_y, bool fixed) override;
    t_api_variant GetRelativeOffset() override;
    t_api_variant GetAbsoluteOffset() override;
    float GetBaseline() override;
    float GetZIndex() override;

    bool IsPointWithinElement(float point_x, float point_y) override;

    bool SetProperty(std::string name, std::string value) override;
    bool RemoveProperty(std::string name) override;
    bool HasProperty(std::string name) override;
    bool HasLocalProperty(std::string name) override;
    std::string GetProperty(std::string name) override;
    std::string GetLocalProperty(std::string name) override;
    float GetPropertyAbsoluteValue(std::string name) override;

    t_api_variant GetContainingBlock() override;

    ElemBase* GetFocusedElement() override;

    std::string GetTagName() override;
    std::string GetId() override;
    void SetId(std::string id) override;

    void SetAttribute(std::string name, std::string value) override;
    bool RemoveAttribute(std::string name) override;
    bool HasAttribute(std::string name) override;
    std::string GetAttribute(std::string name) override;
    std::unordered_map<std::string, t_api_variant> GetAttributes() override;

    float GetAbsoluteLeft() override;
    float GetAbsoluteTop() override;
    float GetClientLeft() override;
    float GetClientTop() override;
    float GetClientWidth() override;
    float GetClientHeight() override;
    ElemBase* GetOffsetParent() override;
    float GetOffsetLeft() override;
    float GetOffsetTop() override;
    float GetOffsetWidth() override;
    float GetOffsetHeight() override;

    float GetScrollLeft() override;
    void SetScrollLeft(float value) override;
    float GetScrollTop() override;
    void SetScrollTop(float value) override;
    float GetScrollWidth() override;
    float GetScrollHeight() override;

    bool IsVisible() override;

    ElemBase* GetParent() override;
    ElemBase* GetClosest(std::string selectors) override;
    ElemBase* GetNextSibling() override;
    ElemBase* GetPreviousSibling() override;
    ElemBase* GetFirstChild() override;
    ElemBase* GetLastChild() override;
    ElemBase* GetChild(int index) override;
    int GetChildCount() override;
    ElemBase* AppendChild(ElemBase* new_element) override;
    ElemBase* InsertBefore(ElemBase* new_element, ElemBase* adjacent_element) override;
    ElemBase* ReplaceChild(ElemBase* new_element, ElemBase* old_element) override;
    ElemBase* RemoveChild(ElemBase* child) override;
    bool HasChildren() override;

    std::string GetInnerRML() override;
    void SetInnerRML(std::string value) override;

    bool Focus() override;
    void Blur() override;
    void Click() override;
    void ScrollIntoView(bool align_with_top = true) override;

    ElemBase* GetElementById(std::string id) override;
    std::vector<ElemBase*> GetElementsByTagName(std::string tag) override;
    std::vector<ElemBase*> GetElementsByClassName(std::string tag) override;
    ElemBase* QuerySelector(std::string selector) override;
    std::vector<ElemBase*> QuerySelectorAll(std::string selector) override;

    ElemBase* GetOwnerDocument() override;

    std::string FormControlGetValue() override;
    void FormControlSetValue(std::string value) override;
};

class Doc : public virtual DocBase, public Elem
{
public:
    Doc(Rml::ElementDocument* elem) : Elem(static_cast<Rml::Element*>(elem)) {}

    void SetVisible(bool toggle) override;
    void Update() override;

    DocBase* GetBody() override;
    ElemBase* CreateElement(std::string name) override;
    ElemBase* CreateTextNode(std::string text) override;
};

namespace api
{
    t_api_variant FromRmlVariant(const Rml::Variant& rml_variant);
    ElemBase* GetOrCreateElement(Rml::Element* rml_element);
    ElemBase* GetOrCreateElement(Rml::ElementPtr* rml_element_ptr);
}