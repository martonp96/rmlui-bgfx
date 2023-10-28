#include <cstdio>
#include <string>

#include "api/api.h"

rml::element_ptr::handle elem = nullptr;
window::handle wnd = nullptr;

bool event_handler(rml::handle element, unsigned short id, const char* name, rml::dictionary::handle parameters, bool interruptible)
{
    printf("%x, %d, %s, %d, %d\n", element, id, name, interruptible, rml::dictionary::get_dict_size(parameters));

    if(std::string(name) == "click" && elem)
    {
        auto size = rml::dictionary::get_dict_size(parameters);
        auto variant_x = rml::dictionary::get_value_by_key(parameters, "mouse_x");
        auto variant_y = rml::dictionary::get_value_by_key(parameters, "mouse_y");

        auto mouse_x = rml::variant::to_double(variant_x);
        auto mouse_y = rml::variant::to_double(variant_y);

        printf("%f, %f\n", mouse_x, mouse_y);

        rml::elem::api::set_property(rml::element_ptr::get_ptr(elem), "left", (std::to_string(mouse_x) + "px").c_str());
        rml::elem::api::set_property(rml::element_ptr::get_ptr(elem), "top", (std::to_string(mouse_y) + "px").c_str());
        rml::elem::api::set_property(rml::element_ptr::get_ptr(elem), "position", "absolute");
    }

    /*for(int i = 0; i < rml::dictionary::get_dict_size(parameters); i++)
    {
        printf("%s\n", rml::dictionary::get_key(parameters, i));
    }*/

    return true;
}

void RenderInitHandler()
{
    auto doc = window::api::load_document(wnd, "main.rml");
    rml::doc::api::set_visible(doc, true);

    elem = rml::doc::api::create_text_node(window::api::get_document(wnd), "test");

    rml::elem::api::append_child(window::api::get_document(wnd), rml::element_ptr::get_ptr(elem));
}


int main()
{
    wnd = window::api::create(1800, 890);

    window::api::register_event_handler(wnd, event_handler);
    window::api::register_render_init_event_handler(wnd, RenderInitHandler);

    window::api::start(wnd);
    
    while (window::api::is_running(wnd))
        window::api::run_loop(wnd);

    window::api::destroy(wnd);

    return 0;
}