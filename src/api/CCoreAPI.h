#pragma once

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

#if defined(_WIN32)
#define API_HANDLE(ns) namespace ns{ typedef struct {} *handle; }
#else
#define API_HANDLE(ns) namespace ns{ typedef void* handle; }
#endif

API_HANDLE(rml);
API_HANDLE(window);
API_HANDLE(rml::variant);
API_HANDLE(rml::dictionary);
API_HANDLE(rml::ptr_array);
API_HANDLE(rml::element_ptr);

namespace window::api
{
    typedef bool(*t_event_handler)(rml::handle element, unsigned short id, const char* name, rml::dictionary::handle parameters, bool interruptible);

    API_FUNC handle create(int width, int height);
    API_FUNC void destroy(handle window);
    API_FUNC bool is_running(handle window);
    API_FUNC bool is_ready(handle window);
    API_FUNC void run_loop(handle window);

    API_FUNC rml::handle create_document(handle window, const char* rml);
    API_FUNC rml::handle load_document(handle window, const char* path);
    API_FUNC rml::handle get_document(handle window);

    API_FUNC void register_event_handler(window::handle window, t_event_handler handler);
}

namespace rml::doc::api
{
    API_FUNC void set_visible(handle document, bool toggle);
    API_FUNC void update(handle document);

    API_FUNC rml::handle get_body(handle document);
    API_FUNC element_ptr::handle create_element(handle document, const char* name);
    API_FUNC element_ptr::handle create_text_node(handle document, const char* text);
}

namespace rml::element_ptr
{
    API_FUNC rml::handle get_ptr(handle ptr);
    API_FUNC void destroy_ptr(handle ptr);
}

namespace rml::variant
{
    struct vec2 { float x, y; };
    struct vec3 { float x, y, z; };
    struct vec4 { float x, y, z, w; };
    struct fcolor { float r, g, b, a; };
    struct color { unsigned char r, g, b, a; };
    
    API_FUNC int get_type(handle variant);
    API_FUNC bool to_bool(handle variant);
    API_FUNC long long to_int(handle variant);
    API_FUNC unsigned long long to_uint(handle variant);
    API_FUNC double to_double(handle variant);
    API_FUNC char* to_string(handle variant);
    API_FUNC vec2 to_vec2(handle variant);
    API_FUNC vec3 to_vec3(handle variant);
    API_FUNC vec4 to_vec4(handle variant);
    API_FUNC fcolor to_fcolor(handle variant);
    API_FUNC color to_color(handle variant);
    API_FUNC void* to_void_ptr(handle variant);
}

namespace rml::dictionary
{
    API_FUNC int get_dict_size(handle dictionary);
    API_FUNC char* get_key(handle dictionary, int at);
    API_FUNC variant::handle get_value_by_id(handle dictionary, int at);
    API_FUNC variant::handle get_value_by_key(handle dictionary, const char* key);
}

namespace rml::ptr_array
{
    API_FUNC int get_arr_size(handle arr);
    API_FUNC void* get_at(handle arr, int at);
    API_FUNC void destroy_arr(handle arr);
}

namespace rml::elem::api
{
    API_FUNC bool add_class(handle element, const char* name);
    API_FUNC bool remove_class(handle element, const char* name);
    API_FUNC bool has_class(handle element, const char* name);
    API_FUNC char* get_class_list(handle element);

    API_FUNC bool add_pseudo_class(handle element, const char* name);
    API_FUNC bool remove_pseudo_class(handle element, const char* name);
    API_FUNC bool has_pseudo_class(handle element, const char* name);
    API_FUNC ptr_array::handle get_pseudo_class_list(handle element);

    API_FUNC void set_offset(handle element, handle offset_parent, float offset_x, float offset_y, bool fixed);
    API_FUNC float get_relative_offset_x(handle element);
    API_FUNC float get_relative_offset_y(handle element);
    API_FUNC float get_baseline(handle element);
    API_FUNC float get_z_index(handle element);

    API_FUNC bool is_point_within_element(handle element, float point_x, float point_y);

    API_FUNC bool set_property(handle element, const char* name, const char* value);
    API_FUNC bool remove_property(handle element, const char* name);
    API_FUNC bool has_property(handle element, const char* name);
    API_FUNC bool has_local_property(handle element, const char* name);
    API_FUNC char* get_property(handle element, const char* name);
    API_FUNC char* get_local_property(handle element, const char* name);
    API_FUNC float get_property_absolute_value(handle element, const char* name);

    API_FUNC float get_containing_block_x(handle element);
    API_FUNC float get_containing_block_y(handle element);

    API_FUNC handle get_focused_element(handle element);

    API_FUNC char* get_tag_name(handle element);
    API_FUNC char* get_id(handle element);
    API_FUNC void set_id(handle element, const char* id);

    API_FUNC void set_attribute(handle element, const char* name, const char* value);
    API_FUNC bool remove_attribute(handle element, const char* name);
    API_FUNC bool has_attribute(handle element, const char* name);
    API_FUNC char* get_attribute(handle element, const char* name);
    API_FUNC dictionary::handle get_attributes(handle element);

    API_FUNC float get_absolute_left(handle element);
    API_FUNC float get_absolute_top(handle element);
    API_FUNC float get_client_left(handle element);
    API_FUNC float get_client_top(handle element);
    API_FUNC float get_client_width(handle element);
    API_FUNC float get_client_height(handle element);
    API_FUNC handle get_offset_parent(handle element);
    API_FUNC float get_offset_left(handle element);
    API_FUNC float get_offset_top(handle element);
    API_FUNC float get_offset_width(handle element);
    API_FUNC float get_offset_height(handle element);

    API_FUNC float get_scroll_left(handle element);
    API_FUNC void set_scroll_left(handle element, float value);
    API_FUNC float get_scroll_top(handle element);
    API_FUNC void set_scroll_top(handle element, float value);
    API_FUNC float get_scroll_width(handle element);
    API_FUNC float get_scroll_height(handle element);

    API_FUNC bool is_visible(handle element);

    API_FUNC handle get_parent(handle element);
    API_FUNC handle get_closest(handle element, const char* selectors);
    API_FUNC handle get_next_sibling(handle element);
    API_FUNC handle get_previous_sibling(handle element);
    API_FUNC handle get_first_child(handle element);
    API_FUNC handle get_last_child(handle element);
    API_FUNC handle get_child(handle element, int index);
    API_FUNC int get_child_count(handle element);
    API_FUNC void append_child(handle element, handle new_element);
    API_FUNC void insert_before(handle element, element_ptr::handle new_element, handle adjacent_element);
    API_FUNC void replace_child(handle element, element_ptr::handle new_element, handle old_element);
    API_FUNC void remove_child(handle element, handle child);
    API_FUNC bool has_children(handle element);

    API_FUNC char* get_inner_rml(handle element);
    API_FUNC void set_inner_rml(handle element, const char* value);

    API_FUNC bool focus(handle element);
    API_FUNC void blur(handle element);
    API_FUNC void click(handle element);
    API_FUNC void scroll_into_view(handle element, bool align_with_top = true);

    API_FUNC handle get_element_by_id(handle element, const char* id);
    API_FUNC ptr_array::handle get_elements_by_tag_name(handle element, const char* tag);
    API_FUNC ptr_array::handle get_elements_by_class_name(handle element, const char* tag);
    API_FUNC handle query_selector(handle element, const char* selector);
    API_FUNC ptr_array::handle query_selector_all(handle element, const char* selector);

    API_FUNC handle get_owner_document(handle element);
}