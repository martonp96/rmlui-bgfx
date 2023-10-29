#include "pch.h"
#include "api.h"
#include "window/CWindow.h"

using namespace window;

app::handle app::create(int width, int height)
{
	return reinterpret_cast<handle>(new CWindow(Eigen::Vector4i{ 200, 200, width, height }));;
}

void app::destroy(app::handle app)
{
	delete app;
}

void app::start(handle app)
{
	reinterpret_cast<CWindow*>(app)->Start();
}

bool app::is_running(handle app)
{
	return reinterpret_cast<CWindow*>(app)->IsRunning();
}

bool app::is_ready(handle app)
{
	return reinterpret_cast<CWindow*>(app)->IsReady();
}

void app::run_loop(handle app)
{
	reinterpret_cast<CWindow*>(app)->Loop();
}

rml::handle app::create_document(app::handle app, const char* rml)
{
	const auto doc = reinterpret_cast<CWindow*>(app)->GetBGFX()->GetRML()->CreateDocumentFromMemory(rml);
	return reinterpret_cast<rml::handle>(doc);
}

rml::handle app::load_document(app::handle app, const char* path)
{
	const auto doc = reinterpret_cast<CWindow*>(app)->GetBGFX()->GetRML()->CreateDocument(path);
	return reinterpret_cast<rml::handle>(doc);
}

rml::handle app::get_document(app::handle app)
{
	const auto doc = reinterpret_cast<CWindow*>(app)->GetBGFX()->GetRML()->GetDocument();
	return reinterpret_cast<rml::handle>(doc);
}

void app::register_event_handler(app::handle app, t_rml_event_handler handler)
{
	const auto wnd = reinterpret_cast<CWindow*>(app);
	wnd->RegisterEventHandler(handler);
}

void app::register_render_event_handler(app::handle app, t_generic_event_handler handler)
{
	const auto wnd = reinterpret_cast<CWindow*>(app);
	wnd->RegisterRenderEventHandler(handler);
}

void app::register_update_event_handler(app::handle app, t_generic_event_handler handler)
{
	const auto wnd = reinterpret_cast<CWindow*>(app);
	wnd->RegisterUpdateEventHandler(handler);
}

void app::register_window_init_event_handler(app::handle app, t_generic_event_handler handler)
{
	const auto wnd = reinterpret_cast<CWindow*>(app);
	wnd->RegisterWindowInitEventHandler(handler);
}

void app::register_render_init_event_handler(app::handle app, t_generic_event_handler handler)
{
	const auto wnd = reinterpret_cast<CWindow*>(app);
	wnd->RegisterRenderInitEventHandler(handler);
}

void rml::doc::set_visible(handle document, bool toggle)
{
	const auto doc = reinterpret_cast<Rml::ElementDocument*>(document);
	toggle ? doc->Show() : doc->Hide();
}

void rml::doc::update(handle document)
{
	reinterpret_cast<Rml::ElementDocument*>(document)->UpdateDocument();
}

rml::handle rml::doc::get_body(handle document)
{
	return document;
}

rml::elem_ptr::handle rml::doc::create_element(handle document, const char* name)
{
	const auto doc = reinterpret_cast<Rml::ElementDocument*>(document);
	return reinterpret_cast<rml::elem_ptr::handle>(new Rml::ElementPtr(doc->CreateElement(name)));
}

rml::elem_ptr::handle rml::doc::create_text_node(handle document, const char* text)
{
	const auto doc = reinterpret_cast<Rml::ElementDocument*>(document);
	return reinterpret_cast<rml::elem_ptr::handle>(new Rml::ElementPtr(doc->CreateTextNode(text)));
}

bool rml::elem::add_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (elem->IsClassSet(name)) return false;
	elem->SetClass(name, true);
	return true;
}

bool rml::elem::remove_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (!elem->IsClassSet(name)) return false;
	elem->SetClass(name, false);
	return true;
}

bool rml::elem::has_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->IsClassSet(name);
}

char* rml::elem::get_class_list(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	Rml::String classListStr = elem->GetClassNames();
	return (char*)elem->GetClassNames().c_str();
}

bool rml::elem::add_pseudo_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (elem->IsPseudoClassSet(name)) return false;
	elem->SetPseudoClass(name, true);
	return true;
}

bool rml::elem::remove_pseudo_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (!elem->IsPseudoClassSet(name)) return false;
	elem->SetPseudoClass(name, false);
	return true;
}

bool rml::elem::has_pseudo_class(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->IsPseudoClassSet(name);
}

rml::ptr_array::handle rml::elem::get_pseudo_class_list(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	auto classList = elem->GetActivePseudoClasses();

	auto vec = new std::vector<void*>();
	
	if (classList.size())
	{
		for (auto& name : classList)
			vec->push_back((void*)name.c_str());
	}

	return reinterpret_cast<ptr_array::handle>(vec);
}

void rml::elem::set_offset(handle element, handle offset_parent, float offset_x, float offset_y, bool fixed)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto offset_elem = reinterpret_cast<Rml::Element*>(offset_parent);
	elem->SetOffset({ offset_x, offset_y }, offset_elem, fixed);
}

float rml::elem::get_relative_offset_x(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetRelativeOffset().x;
}

float rml::elem::get_relative_offset_y(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetRelativeOffset().y;
}

float rml::elem::get_baseline(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetBaseline();
}

float rml::elem::get_z_index(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetZIndex();
}

bool rml::elem::is_point_within_element(handle element, float point_x, float point_y)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->IsPointWithinElement({ point_x, point_y });
}

bool rml::elem::set_property(handle element, const char* name, const char* value)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->SetProperty(name, value);
}

bool rml::elem::remove_property(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (!elem->GetProperty(name))return false;
	elem->RemoveProperty(name);
	return true;
}

bool rml::elem::has_property(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetProperty(name) != nullptr;
}

bool rml::elem::has_local_property(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetLocalProperty(name) != nullptr;
}

char* rml::elem::get_property(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto prop = elem->GetProperty(name);

	if (!prop) return nullptr;
	return (char*)prop->ToString().c_str();
}

char* rml::elem::get_local_property(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto prop = elem->GetLocalProperty(name);

	if (!prop) return nullptr;
	return (char*)prop->ToString().c_str();
}

float rml::elem::get_property_absolute_value(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (elem->GetProperty(name) == nullptr) return 0.f;
	return elem->ResolveNumericProperty(name);
}

float rml::elem::get_containing_block_x(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetContainingBlock().x;
}

float rml::elem::get_containing_block_y(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetContainingBlock().y;
}

rml::handle rml::elem::get_focused_element(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<rml::handle>(elem->GetFocusLeafNode());
}

char* rml::elem::get_tag_name(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return (char*)elem->GetTagName().c_str();
}

char* rml::elem::get_id(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return (char*)elem->GetId().c_str();
}

void rml::elem::set_id(handle element, const char* id)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->SetId(id);
}

void rml::elem::set_attribute(handle element, const char* name, const char* value)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->SetAttribute(name, value);
}

bool rml::elem::remove_attribute(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	if (!elem->HasAttribute(name)) return false;
	elem->RemoveAttribute(name);
	return true;
}

bool rml::elem::has_attribute(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->HasAttribute(name);
}

char* rml::elem::get_attribute(handle element, const char* name)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return (char*)elem->GetAttribute(name);
}

rml::dict::handle rml::elem::get_attributes(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto& attributes = elem->GetAttributes();
	return reinterpret_cast<dict::handle>((void*)&attributes);
}

float rml::elem::get_absolute_left(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetAbsoluteLeft();
}

float rml::elem::get_absolute_top(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetAbsoluteTop();
}

float rml::elem::get_client_left(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetClientLeft();
}

float rml::elem::get_client_top(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetClientTop();
}

float rml::elem::get_client_width(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetClientWidth();
}

float rml::elem::get_client_height(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetClientHeight();
}

rml::handle rml::elem::get_offset_parent(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetOffsetParent());
}

float rml::elem::get_offset_left(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetOffsetLeft();
}

float rml::elem::get_offset_top(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetOffsetTop();
}

float rml::elem::get_offset_width(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetOffsetWidth();
}

float rml::elem::get_offset_height(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetOffsetHeight();
}

float rml::elem::get_scroll_left(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetScrollLeft();
}

void rml::elem::set_scroll_left(handle element, float value)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->SetScrollLeft(value);
}

float rml::elem::get_scroll_top(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetScrollTop();
}

void rml::elem::set_scroll_top(handle element, float value)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->SetScrollTop(value);
}

float rml::elem::get_scroll_width(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetScrollWidth();
}

float rml::elem::get_scroll_height(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetScrollHeight();
}

bool rml::elem::is_visible(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->IsVisible();
}

rml::handle rml::elem::get_parent(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetParentNode());
}

rml::handle rml::elem::get_closest(handle element, const char* selectors)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->Closest(selectors));
}

rml::handle rml::elem::get_next_sibling(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetNextSibling());
}

rml::handle rml::elem::get_previous_sibling(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetPreviousSibling());
}

rml::handle rml::elem::get_first_child(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetFirstChild());
}

rml::handle rml::elem::get_last_child(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetLastChild());
}

rml::handle rml::elem::get_child(handle element, int index)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetChild(index));
}

int rml::elem::get_child_count(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->GetNumChildren();
}

rml::handle rml::elem::append_child(handle element, handle new_element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto child = reinterpret_cast<Rml::Element*>(new_element);

	Rml::ElementPtr oldElement(child);
	const auto ret = reinterpret_cast<handle>(elem->AppendChild(std::move(oldElement)));
	oldElement.release();
	return ret;
}

rml::handle rml::elem::insert_before(handle element, handle new_element, handle adjacent_element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto child = reinterpret_cast<Rml::Element*>(new_element);
	const auto adjacent = reinterpret_cast<Rml::Element*>(adjacent_element);

	Rml::ElementPtr oldElement(child);
	const auto ret = reinterpret_cast<handle>(elem->InsertBefore(std::move(oldElement), adjacent));
	oldElement.release();
	return ret;
}

rml::handle rml::elem::replace_child(handle element, handle new_element, handle old_element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto child = reinterpret_cast<Rml::Element*>(new_element);
	const auto old = reinterpret_cast<Rml::Element*>(old_element);

	Rml::ElementPtr oldElement(child);
	auto ret_ptr = elem->ReplaceChild(std::move(oldElement), old);
	const auto ret = ret_ptr.get();
	ret_ptr.release();
	oldElement.release();
	return reinterpret_cast<rml::handle>(ret);
}

rml::handle rml::elem::remove_child(handle element, handle child_element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	const auto child = reinterpret_cast<Rml::Element*>(child_element);
	auto ret_ptr = elem->RemoveChild(child);
	const auto ret = ret_ptr.get();
	ret_ptr.release();
	return reinterpret_cast<rml::handle>(ret);
}

bool rml::elem::has_children(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->HasChildNodes();
}

char* rml::elem::get_inner_rml(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return (char*)elem->GetInnerRML().c_str();
}

void rml::elem::set_inner_rml(handle element, const char* value)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->SetInnerRML(value);
}

bool rml::elem::focus(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->Focus();
}

void rml::elem::blur(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->Blur();
}

void rml::elem::click(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return elem->Click();
}

void rml::elem::scroll_into_view(handle element, bool align_with_top)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	elem->ScrollIntoView(align_with_top);
}

rml::handle rml::elem::get_element_by_id(handle element, const char* id)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetElementById(id));
}

rml::ptr_array::handle rml::elem::get_elements_by_tag_name(handle element, const char* tag)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	Rml::ElementList* elements = new Rml::ElementList();
	elem->GetElementsByTagName(*elements, tag);
	return reinterpret_cast<ptr_array::handle>(elements);
}

rml::ptr_array::handle rml::elem::get_elements_by_class_name(handle element, const char* tag)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	Rml::ElementList* elements = new Rml::ElementList();
	elem->GetElementsByClassName(*elements, tag);
	return reinterpret_cast<ptr_array::handle>(elements);
}

rml::handle rml::elem::query_selector(handle element, const char* selector)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->QuerySelector(selector));
}

rml::ptr_array::handle rml::elem::query_selector_all(handle element, const char* selector)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	Rml::ElementList* elements = new Rml::ElementList();
	elem->QuerySelectorAll(*elements, selector);
	return reinterpret_cast<ptr_array::handle>(elements);
}

rml::handle rml::elem::get_owner_document(handle element)
{
	const auto elem = reinterpret_cast<Rml::Element*>(element);
	return reinterpret_cast<handle>(elem->GetOwnerDocument());
}

char* rml::elem::form_control_get_value(handle element)
{
	const auto elem = reinterpret_cast<Rml::ElementFormControl*>(element);
	return (char*)elem->GetValue().c_str();
}


void rml::elem::form_control_set_value(handle element, const char* value)
{
	const auto elem = reinterpret_cast<Rml::ElementFormControl*>(element);
	elem->SetValue(value);
}

int rml::variant::get_type(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->GetType();
}

bool rml::variant::to_bool(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<bool>();
}

long long rml::variant::to_int(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<long long>();
}

unsigned long long rml::variant::to_uint(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<unsigned long long>();
}

double rml::variant::to_double(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<double>();
}

char* rml::variant::to_string(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<char*>();
}

rml::variant::vec2 rml::variant::to_vec2(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<rml::variant::vec2>();
}

rml::variant::vec3 rml::variant::to_vec3(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<rml::variant::vec3>();
}

rml::variant::vec4 rml::variant::to_vec4(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<rml::variant::vec4>();
}

rml::variant::fcolor rml::variant::to_fcolor(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<rml::variant::fcolor>();
}

rml::variant::color rml::variant::to_color(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<rml::variant::color>();
}

void* rml::variant::to_void_ptr(handle variant)
{
	return reinterpret_cast<Rml::Variant*>(variant)->Get<void*>();
}

int rml::dict::get_dict_size(handle dictionary)
{
	return reinterpret_cast<Rml::Dictionary*>(dictionary)->size();
}

char* rml::dict::get_key(handle dictionary, int at)
{
	const auto dict = reinterpret_cast<Rml::Dictionary*>(dictionary);
	if (at >= dict->size())
		return nullptr;

	const auto it = dict->begin() + at;
	return (char*)it->first.c_str();
}

rml::variant::handle rml::dict::get_value_by_id(handle dictionary, int at)
{
	const auto dict = reinterpret_cast<Rml::Dictionary*>(dictionary);
	if (at >= dict->size())
		return nullptr;

	const auto it = dict->begin() + at;
	return reinterpret_cast<rml::variant::handle>(&it->second);
}

rml::variant::handle rml::dict::get_value_by_key(handle dictionary, const char* key)
{
	const auto dict = reinterpret_cast<Rml::Dictionary*>(dictionary);

	if(const auto it = dict->find(key); it != dict->end())
	    return reinterpret_cast<rml::variant::handle>(&it->second);

	return nullptr;
}

int rml::ptr_array::get_arr_size(handle arr)
{
	const auto vec_ptr = reinterpret_cast<std::vector<void*>*>(arr);
	return vec_ptr->size();
}

void* rml::ptr_array::get_at(handle arr, int at)
{
	const auto vec_ptr = reinterpret_cast<std::vector<void*>*>(arr);
	if (vec_ptr->size() >= at)
		return nullptr;
	return vec_ptr->at(at);
}

void rml::ptr_array::destroy_arr(handle arr)
{
	delete reinterpret_cast<std::vector<void*>*>(arr);
}

rml::handle rml::elem_ptr::get_ptr(handle ptr)
{
	const auto elem = reinterpret_cast<Rml::ElementPtr*>(ptr)->get();
	return reinterpret_cast<rml::handle>(elem);
}

void rml::elem_ptr::destroy_ptr(handle ptr)
{
	delete reinterpret_cast<Rml::ElementPtr*>(ptr);
}

file::handle app::file_open(const char* path)
{
	return reinterpret_cast<file::handle>(Rml::GetFileInterface()->Open(path));
}

void app::file_close(file::handle file)
{
	Rml::GetFileInterface()->Close(reinterpret_cast<Rml::FileHandle>(file));
}

unsigned long long app::file_read(file::handle file, void* buffer, unsigned long long size)
{
	return Rml::GetFileInterface()->Read(buffer, size, reinterpret_cast<Rml::FileHandle>(file));
}

bool app::file_seek(file::handle file, long offset, int origin)
{
	return Rml::GetFileInterface()->Seek(reinterpret_cast<Rml::FileHandle>(file), offset, origin);
}

unsigned long long app::file_tell(file::handle file)
{
	return Rml::GetFileInterface()->Tell(reinterpret_cast<Rml::FileHandle>(file));
}

unsigned long long app::file_length(file::handle file)
{
	return Rml::GetFileInterface()->Length(reinterpret_cast<Rml::FileHandle>(file));
}

void app::load_font_face(const char* path, bool is_default)
{
	Rml::LoadFontFace(path, is_default);
}
