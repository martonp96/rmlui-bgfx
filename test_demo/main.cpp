#include <cstdio>
#include <string>

#include "api/api.h"

using namespace window;

rml::element_ptr::handle elem = nullptr;
window::handle wnd = nullptr;
rml::handle doc = nullptr;

bool event_handler(rml::handle element, unsigned short id, const char* name, rml::dictionary::handle parameters, bool interruptible)
{
    printf("%x, %d, %s, %d, %d\n", element, id, name, interruptible, rml::dictionary::get_dict_size(parameters));

    /*for(int i = 0; i < rml::dictionary::get_dict_size(parameters); i++)
    {
        printf("%s\n", rml::dictionary::get_key(parameters, i));
    }*/

    return true;
}

void RenderInitHandler()
{
    window::api::load_font_face("assets/LatoLatin-Regular.ttf", false);
    window::api::load_font_face("assets/LatoLatin-Italic.ttf", false);
    window::api::load_font_face("assets/LatoLatin-Bold.ttf", false);
    window::api::load_font_face("assets/LatoLatin-BoldItalic.ttf", false);
    window::api::load_font_face("assets/NotoEmoji-Regular.ttf", true);

    doc = window::api::load_document(wnd, "basic/demo/data/demo.rml");
    if(doc)
    {
        const auto title_elem = rml::elem::api::get_element_by_id(doc, "title");
        rml::elem::api::set_inner_rml(title_elem, "test title");

        if(const auto sandbox_src = rml::elem::api::get_element_by_id(doc, "sandbox_rml_source"))
        {
            std::string value(rml::elem::api::form_control_get_value(sandbox_src));
            value += "<p>Write your RML here</p>\n\n<!-- <img src=\"assets/high_scores_alien_1.tga\"/> -->";
            rml::elem::api::form_control_set_value(sandbox_src, value.c_str());
        }

        if (const auto sandbox_tgt = rml::elem::api::get_element_by_id(doc, "sandbox_target"))
        {
            const auto iframe_doc = window::api::create_document(wnd, "");
            const auto parent_node = rml::elem::api::get_parent(iframe_doc);
            const auto iframe_ptr = rml::elem::api::remove_child(parent_node, iframe_doc);

            rml::elem::api::append_child(sandbox_tgt, iframe_ptr);
            rml::elem::api::set_property(iframe_doc, "position", "absolute");
            rml::elem::api::set_property(iframe_doc, "display", "block");
            rml::elem::api::set_inner_rml(iframe_doc, "<p>Rendered output goes here.</p>");
        }

        rml::doc::api::set_visible(doc, true);
    }
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