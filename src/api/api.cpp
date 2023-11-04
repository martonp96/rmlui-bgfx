#include "pch.h"
#include "api.h"
#include "window/CWindow.h"

using namespace window;

static std::unordered_map<Rml::Element*, ElemBase*> g_element_map;

ElemBase* api::GetOrCreateElement(Rml::Element* rml_element)
{
	if (g_element_map.find(rml_element) != g_element_map.end())
		return g_element_map[rml_element];

	const auto elem = new Elem(rml_element);
	g_element_map[rml_element] = elem;
	return elem;
}

ElemBase* api::GetOrCreateElement(Rml::ElementPtr* rml_element_ptr)
{
	const auto rml_element = rml_element_ptr->get();
	return api::GetOrCreateElement(rml_element);
}

AppBase* Core::CreateApp(int width, int height)
{
	return new App(width, height);
}

App::App(int width, int height)
{
	m_wnd = new CWindow(Eigen::Vector4i{ 200, 200, width, height });;
}

void App::Destroy()
{
	delete m_wnd;
}

void App::Start()
{
	m_wnd->Start();
}

bool App::IsRunning()
{
	return m_wnd->IsRunning();
}

bool App::IsReady()
{
	return m_wnd->IsReady();
}

void App::RunLoop()
{
	m_wnd->Loop();
}

DocBase* App::CreateDocument(std::string rml)
{
	const auto doc = m_wnd->GetBGFX()->GetRML()->CreateDocumentFromMemory(rml);
	return new Doc(doc);
}

DocBase* App::LoadDocument(std::string path)
{
	const auto doc = m_wnd->GetBGFX()->GetRML()->CreateDocument(path);
	return new Doc(doc);
}

DocBase* App::GetDocument()
{
	const auto doc = m_wnd->GetBGFX()->GetRML()->GetDocument();
	return new Doc(doc);
}

void App::RegisterRmlEventHandler(const t_rml_event_handler& handler)
{
	const auto wnd = m_wnd;
	wnd->RegisterEventHandler(handler);
}

void App::RegisterRenderEventHandler(const t_generic_event_handler& handler)
{
	const auto wnd = m_wnd;
	wnd->RegisterRenderEventHandler(handler);
}

void App::RegisterUpdateEventHandler(const t_generic_event_handler& handler)
{
	const auto wnd = m_wnd;
	wnd->RegisterUpdateEventHandler(handler);
}

void App::RegisterWindowInitEventHandler(const t_generic_event_handler& handler)
{
	const auto wnd = m_wnd;
	wnd->RegisterWindowInitEventHandler(handler);
}

void App::RegisterRenderInitEventHandler(const t_generic_event_handler& handler)
{
	const auto wnd = m_wnd;
	wnd->RegisterRenderInitEventHandler(handler);
}

void App::LoadFontFace(std::string path, bool is_default)
{
	Rml::LoadFontFace(path, is_default);
}

void Doc::SetVisible(bool toggle)
{
	toggle ? static_cast<Rml::ElementDocument*>(element)->Show() : static_cast<Rml::ElementDocument*>(element)->Hide();
}

void Doc::Update()
{
	static_cast<Rml::ElementDocument*>(element)->UpdateDocument();
}

DocBase* Doc::GetBody()
{
	return this;
}

ElemBase* Doc::CreateElement(std::string name)
{
	return api::GetOrCreateElement(new Rml::ElementPtr(static_cast<Rml::ElementDocument*>(element)->CreateElement(name)));
}

ElemBase* Doc::CreateTextNode(std::string text)
{
	return api::GetOrCreateElement(new Rml::ElementPtr(static_cast<Rml::ElementDocument*>(element)->CreateTextNode(text)));
}

t_api_variant api::FromRmlVariant(const Rml::Variant& rml_variant)
{
	t_api_variant std_variant;
	Rml::Variant::Type type = rml_variant.GetType();
	switch (type)
	{
	case Rml::Variant::Type::BOOL:
		std_variant = rml_variant.Get<bool>();
		break;
	case Rml::Variant::Type::BYTE:
	case Rml::Variant::Type::UINT:
	case Rml::Variant::Type::UINT64:
		std_variant = rml_variant.Get<uint64_t>();
		break;
	case Rml::Variant::Type::CHAR:
	case Rml::Variant::Type::INT:
	case Rml::Variant::Type::INT64:
		std_variant = rml_variant.Get<int64_t>();
		break;
	case Rml::Variant::Type::FLOAT:
	case Rml::Variant::Type::DOUBLE:
		std_variant = rml_variant.Get<double>();
		break;
	case Rml::Variant::Type::STRING:
		std_variant = rml_variant.Get<Rml::String>();
		SPDLOG_INFO("variant string {}", rml_variant.Get<Rml::String>());
		break;
	case Rml::Variant::Type::VECTOR2:
	{
		auto vec = rml_variant.Get<Rml::Vector2f>();
		std_variant = vec2{ vec.x, vec.y };
		break;
	}
	case Rml::Variant::Type::VECTOR3:
	{
		auto vec = rml_variant.Get<Rml::Vector3f>();
		std_variant = vec3{ vec.x, vec.y, vec.z };
		break;
	}
	case Rml::Variant::Type::VECTOR4:
	{
		auto vec = rml_variant.Get<Rml::Vector4f>();
		std_variant = vec4{ vec.x, vec.y, vec.z, vec.w };
		break;
	}
	case Rml::Variant::Type::COLOURF:
	{
		auto vec = rml_variant.Get<Rml::Colourf>();
		std_variant = fcolor{ vec.red, vec.green, vec.blue, vec.alpha };
		break;
	}
	case Rml::Variant::Type::COLOURB:
	{
		auto vec = rml_variant.Get<Rml::Colourb>();
		std_variant = color{ vec.red, vec.green, vec.blue, vec.alpha };
		break;
	}
	case Rml::Variant::Type::NONE:
		SPDLOG_ERROR("Received none variant type in rmlui event: {}", (int)type);
		std_variant = false;
		break;
	case Rml::Variant::Type::VOIDPTR:
		std_variant = static_cast<ElemBase*>(rml_variant.Get<void*>());
		break;
	default:
		SPDLOG_ERROR("Received unhandled variant type in rmlui event: {}", (int)type);
		break;
	}
	return std_variant;
}

Elem::Elem(Rml::Element* elem) : element(elem)
{

}

/*
Elem::Elem(Rml::ElementPtr* elem) : element(elem->get()), element_ptr(elem)
{

}
*/

bool Elem::AddClass(std::string name)
{
	const auto elem = element;
	if (elem->IsClassSet(name)) return false;
	elem->SetClass(name, true);
	return true;
}

bool Elem::RemoveClass(std::string name)
{
	const auto elem = element;
	if (!elem->IsClassSet(name)) return false;
	elem->SetClass(name, false);
	return true;
}

bool Elem::HasClass(std::string name)
{
	const auto elem = element;
	return elem->IsClassSet(name);
}

std::string Elem::GetClassList()
{
	const auto elem = element;
	Rml::String classListStr = elem->GetClassNames();
	return elem->GetClassNames();
}

bool Elem::AddPseudoClass(std::string name)
{
	const auto elem = element;
	if (elem->IsPseudoClassSet(name)) return false;
	elem->SetPseudoClass(name, true);
	return true;
}

bool Elem::RemovePseudoClass(std::string name)
{
	const auto elem = element;
	if (!elem->IsPseudoClassSet(name)) return false;
	elem->SetPseudoClass(name, false);
	return true;
}

bool Elem::HasPseudoClass(std::string name)
{
	const auto elem = element;
	return elem->IsPseudoClassSet(name);
}

std::vector<std::string> Elem::GetPseudoClassList()
{
	const auto elem = element;
	auto classList = elem->GetActivePseudoClasses();

	std::vector<std::string> vec;

	if (classList.size())
	{
		for (auto& name : classList)
			vec.push_back(name);
	}

	return vec;
}

void Elem::SetOffset(ElemBase* offset_parent, float offset_x, float offset_y, bool fixed)
{
	const auto elem = element;
	const auto offset_elem = reinterpret_cast<Rml::Element*>(offset_parent);
	elem->SetOffset({ offset_x, offset_y }, offset_elem, fixed);
}

t_api_variant Elem::GetRelativeOffset()
{
	const auto elem = element;
	const auto offs = elem->GetRelativeOffset();
	return vec2{ offs.x, offs.y };
}

t_api_variant Elem::GetAbsoluteOffset()
{
	const auto elem = element;
	const auto offs = elem->GetAbsoluteOffset();
	return vec2{ offs.x, offs.y };
}

float Elem::GetBaseline()
{
	const auto elem = element;
	return elem->GetBaseline();
}

float Elem::GetZIndex()
{
	const auto elem = element;
	return elem->GetZIndex();
}

bool Elem::IsPointWithinElement(float point_x, float point_y)
{
	const auto elem = element;
	return elem->IsPointWithinElement({ point_x, point_y });
}

bool Elem::SetProperty(std::string name, std::string value)
{
	const auto elem = element;
	return elem->SetProperty(name, value);
}

bool Elem::RemoveProperty(std::string name)
{
	const auto elem = element;
	if (!elem->GetProperty(name))return false;
	elem->RemoveProperty(name);
	return true;
}

bool Elem::HasProperty(std::string name)
{
	const auto elem = element;
	return elem->GetProperty(name) != nullptr;
}

bool Elem::HasLocalProperty(std::string name)
{
	const auto elem = element;
	return elem->GetLocalProperty(name) != nullptr;
}

std::string Elem::GetProperty(std::string name)
{
	const auto elem = element;
	const auto prop = elem->GetProperty(name);

	if (!prop) return "";
	return prop->ToString();
}

std::string Elem::GetLocalProperty(std::string name)
{
	const auto elem = element;
	const auto prop = elem->GetLocalProperty(name);

	if (!prop) return "";
	return prop->ToString();
}

float Elem::GetPropertyAbsoluteValue(std::string name)
{
	const auto elem = element;
	if (elem->GetProperty(name) == nullptr) return 0.f;
	return elem->ResolveNumericProperty(name);
}

t_api_variant Elem::GetContainingBlock()
{
	const auto elem = element;
	const auto offs = elem->GetContainingBlock();
	return vec2{ offs.x, offs.y };
}

ElemBase* Elem::GetFocusedElement()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetFocusLeafNode());
}

std::string Elem::GetTagName()
{
	const auto elem = element;
	return elem->GetTagName();
}

std::string Elem::GetId()
{
	const auto elem = element;
	return elem->GetId();
}

void Elem::SetId(std::string id)
{
	const auto elem = element;
	elem->SetId(id);
}

void Elem::SetAttribute(std::string name, std::string value)
{
	const auto elem = element;
	elem->SetAttribute(name, value);
	SPDLOG_INFO("Setattribute {} {}", name, value);
}

bool Elem::RemoveAttribute(std::string name)
{
	const auto elem = element;
	if (!elem->HasAttribute(name)) return false;
	elem->RemoveAttribute(name);
	return true;
}

bool Elem::HasAttribute(std::string name)
{
	const auto elem = element;
	return elem->HasAttribute(name);
}

std::string Elem::GetAttribute(std::string name)
{
	const auto elem = element;
	return elem->GetAttribute(name)->Get<std::string>();
}

std::unordered_map<std::string, t_api_variant> Elem::GetAttributes()
{
	const auto elem = element;
	const auto& attributes = elem->GetAttributes();

	std::unordered_map<std::string, t_api_variant> map;
	for (auto& attr : attributes)
	{
		map[attr.first] = api::FromRmlVariant(attr.second);
	}
	return map;
}

float Elem::GetAbsoluteLeft()
{
	const auto elem = element;
	return elem->GetAbsoluteLeft();
}

float Elem::GetAbsoluteTop()
{
	const auto elem = element;
	return elem->GetAbsoluteTop();
}

float Elem::GetClientLeft()
{
	const auto elem = element;
	return elem->GetClientLeft();
}

float Elem::GetClientTop()
{
	const auto elem = element;
	return elem->GetClientTop();
}

float Elem::GetClientWidth()
{
	const auto elem = element;
	return elem->GetClientWidth();
}

float Elem::GetClientHeight()
{
	const auto elem = element;
	return elem->GetClientHeight();
}

ElemBase* Elem::GetOffsetParent()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetOffsetParent());
}

float Elem::GetOffsetLeft()
{
	const auto elem = element;
	return elem->GetOffsetLeft();
}

float Elem::GetOffsetTop()
{
	const auto elem = element;
	return elem->GetOffsetTop();
}

float Elem::GetOffsetWidth()
{
	const auto elem = element;
	return elem->GetOffsetWidth();
}

float Elem::GetOffsetHeight()
{
	const auto elem = element;
	return elem->GetOffsetHeight();
}

float Elem::GetScrollLeft()
{
	const auto elem = element;
	return elem->GetScrollLeft();
}

void Elem::SetScrollLeft(float value)
{
	const auto elem = element;
	elem->SetScrollLeft(value);
}

float Elem::GetScrollTop()
{
	const auto elem = element;
	return elem->GetScrollTop();
}

void Elem::SetScrollTop(float value)
{
	const auto elem = element;
	elem->SetScrollTop(value);
}

float Elem::GetScrollWidth()
{
	const auto elem = element;
	return elem->GetScrollWidth();
}

float Elem::GetScrollHeight()
{
	const auto elem = element;
	return elem->GetScrollHeight();
}

bool Elem::IsVisible()
{
	const auto elem = element;
	return elem->IsVisible();
}

ElemBase* Elem::GetParent()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetParentNode());
}

ElemBase* Elem::GetClosest(std::string selectors)
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->Closest(selectors));
}

ElemBase* Elem::GetNextSibling()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetNextSibling());
}

ElemBase* Elem::GetPreviousSibling()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetPreviousSibling());
}

ElemBase* Elem::GetFirstChild()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetFirstChild());
}

ElemBase* Elem::GetLastChild()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetLastChild());
}

ElemBase* Elem::GetChild(int index)
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetChild(index));
}

int Elem::GetChildCount()
{
	const auto elem = element;
	return elem->GetNumChildren();
}

ElemBase* Elem::AppendChild(ElemBase* new_element)
{
	const auto elem = element;
	const auto child = dynamic_cast<Elem*>(new_element)->element;

	Rml::ElementPtr oldElement(child);
	const auto ret = api::GetOrCreateElement(elem->AppendChild(std::move(oldElement)));
	oldElement.release();
	return ret;
}

ElemBase* Elem::InsertBefore(ElemBase* new_element, ElemBase* adjacent_element)
{
	const auto elem = element;
	const auto child = dynamic_cast<Elem*>(new_element)->element;
	const auto adjacent = dynamic_cast<Elem*>(adjacent_element)->element;

	Rml::ElementPtr oldElement(child);
	const auto ret = api::GetOrCreateElement(elem->InsertBefore(std::move(oldElement), adjacent));
	oldElement.release();
	return ret;
}

ElemBase* Elem::ReplaceChild(ElemBase* new_element, ElemBase* old_element)
{
	const auto elem = element;
	const auto child = dynamic_cast<Elem*>(new_element)->element;
	const auto old = dynamic_cast<Elem*>(old_element)->element;

	Rml::ElementPtr oldElement(child);
	auto ret_ptr = elem->ReplaceChild(std::move(oldElement), old);
	const auto ret = ret_ptr.get();
	ret_ptr.release();
	oldElement.release();
	return api::GetOrCreateElement(ret);
}

ElemBase* Elem::RemoveChild(ElemBase* child_element)
{
	const auto elem = element;
	const auto child = dynamic_cast<Elem*>(child_element)->element;
	auto ret_ptr = elem->RemoveChild(child);
	const auto ret = ret_ptr.get();
	ret_ptr.release();
	return api::GetOrCreateElement(ret);
}

bool Elem::HasChildren()
{
	const auto elem = element;
	return elem->HasChildNodes();
}

std::string Elem::GetInnerRML()
{
	const auto elem = element;
	return elem->GetInnerRML();
}

void Elem::SetInnerRML(std::string value)
{
	const auto elem = element;
	elem->SetInnerRML(value);
}

bool Elem::Focus()
{
	const auto elem = element;
	return elem->Focus();
}

void Elem::Blur()
{
	const auto elem = element;
	return elem->Blur();
}

void Elem::Click()
{
	const auto elem = element;
	return elem->Click();
}

void Elem::ScrollIntoView(bool align_with_top)
{
	const auto elem = element;
	elem->ScrollIntoView(align_with_top);
}

ElemBase* Elem::GetElementById(std::string id)
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetElementById(id));
}

std::vector<ElemBase*> Elem::GetElementsByTagName(std::string tag)
{
	const auto elem = element;
	Rml::ElementList elements;
	elem->GetElementsByTagName(elements, tag);
	std::vector<ElemBase*> vec;
	for (auto elem : elements)
		vec.push_back(api::GetOrCreateElement(elem));
	return vec;
}

std::vector<ElemBase*> Elem::GetElementsByClassName(std::string tag)
{
	const auto elem = element;
	Rml::ElementList elements;
	elem->GetElementsByClassName(elements, tag);
	std::vector<ElemBase*> vec;
	for (auto elem : elements)
		vec.push_back(api::GetOrCreateElement(elem));
	return vec;
}

ElemBase* Elem::QuerySelector(std::string selector)
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->QuerySelector(selector));
}

std::vector<ElemBase*> Elem::QuerySelectorAll(std::string selector)
{
	const auto elem = element;
	Rml::ElementList elements;
	elem->QuerySelectorAll(elements, selector);
	std::vector<ElemBase*> vec;
	for (auto elem : elements)
		vec.push_back(api::GetOrCreateElement(elem));
	return vec;
}

ElemBase* Elem::GetOwnerDocument()
{
	const auto elem = element;
	return api::GetOrCreateElement(elem->GetOwnerDocument());
}

std::string Elem::FormControlGetValue()
{
	const auto elem = reinterpret_cast<Rml::ElementFormControl*>(element);
	return elem->GetValue();
}

void Elem::FormControlSetValue(std::string value)
{
	const auto elem = reinterpret_cast<Rml::ElementFormControl*>(element);
	elem->SetValue(value);
}