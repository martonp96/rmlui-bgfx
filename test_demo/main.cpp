#include <cstdio>
#include <string>

#include "api/api.h"

rml::elem_ptr::handle elem = nullptr;
app::handle wnd = nullptr;
rml::handle doc = nullptr;

bool event_handler(rml::handle element, unsigned short id, const char* name, rml::dict::handle parameters, bool interruptible)
{
    printf("%x, %d, %s, %d, %d\n", element, id, name, interruptible, rml::dict::get_dict_size(parameters));

    /*for(int i = 0; i < rml::dict::get_dict_size(parameters); i++)
    {
        printf("%s\n", rml::dict::get_key(parameters, i));
    }*/

    return true;
}

void RenderInitHandler()
{
    app::load_font_face("assets/LatoLatin-Regular.ttf", false);
    app::load_font_face("assets/LatoLatin-Italic.ttf", false);
    app::load_font_face("assets/LatoLatin-Bold.ttf", false);
    app::load_font_face("assets/LatoLatin-BoldItalic.ttf", false);
    app::load_font_face("assets/NotoEmoji-Regular.ttf", true);

    doc = app::load_document(wnd, "basic/demo/data/demo.rml");
    if(doc)
    {
        const auto title_elem = rml::elem::get_element_by_id(doc, "title");
        rml::elem::set_inner_rml(title_elem, "test title");

        if(const auto sandbox_src = rml::elem::get_element_by_id(doc, "sandbox_rml_source"))
        {
            std::string value(rml::elem::form_control_get_value(sandbox_src));
            value += "<p>Write your RML here</p>\n\n<!-- <img src=\"assets/high_scores_alien_1.tga\"/> -->";
            rml::elem::form_control_set_value(sandbox_src, value.c_str());
        }

        if (const auto sandbox_tgt = rml::elem::get_element_by_id(doc, "sandbox_target"))
        {
            const auto iframe_doc = app::create_document(wnd, "");
            const auto parent_node = rml::elem::get_parent(iframe_doc);
            const auto iframe_ptr = rml::elem::remove_child(parent_node, iframe_doc);

            rml::elem::append_child(sandbox_tgt, iframe_ptr);
            rml::elem::set_property(iframe_doc, "position", "absolute");
            rml::elem::set_property(iframe_doc, "display", "block");
            rml::elem::set_inner_rml(iframe_doc, "<p>Rendered output goes here.</p>");
        }

        rml::doc::set_visible(doc, true);
    }
}

int main()
{
    wnd = app::create(1800, 890);

    app::register_event_handler(wnd, event_handler);
    app::register_render_init_event_handler(wnd, RenderInitHandler);

    app::start(wnd);
    
    while (app::is_running(wnd))
        app::run_loop(wnd);

    app::destroy(wnd);

    return 0;
}