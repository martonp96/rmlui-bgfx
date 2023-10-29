#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#include <cstdio>
#include <string>

#include "api/api.h"

rml::elem_ptr::handle elem = nullptr;
app::handle wnd = nullptr;

bool event_handler(rml::handle element, unsigned short id, const char* name, rml::dict::handle parameters, bool interruptible)
{
    printf("%x, %d, %s, %d, %d\n", element, id, name, interruptible, rml::dict::get_dict_size(parameters));

    if(std::string(name) == "click" && elem)
    {
        auto size = rml::dict::get_dict_size(parameters);
        auto variant_x = rml::dict::get_value_by_key(parameters, "mouse_x");
        auto variant_y = rml::dict::get_value_by_key(parameters, "mouse_y");

        auto mouse_x = rml::variant::to_double(variant_x);
        auto mouse_y = rml::variant::to_double(variant_y);

        printf("%f, %f\n", mouse_x, mouse_y);

        rml::elem::set_property(rml::elem_ptr::get_ptr(elem), "left", (std::to_string(mouse_x) + "px").c_str());
        rml::elem::set_property(rml::elem_ptr::get_ptr(elem), "top", (std::to_string(mouse_y) + "px").c_str());
        rml::elem::set_property(rml::elem_ptr::get_ptr(elem), "position", "absolute");
    }

    return true;
}

void RenderInitHandler()
{
    app::load_font_face("arial.ttf", true);

    auto doc = app::load_document(wnd, "main.rml");
    rml::doc::set_visible(doc, true);

    elem = rml::doc::create_text_node(app::get_document(wnd), "test");
    rml::elem::append_child(app::get_document(wnd), rml::elem_ptr::get_ptr(elem));
}

int fn_main()
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

void RunMain(const Napi::CallbackInfo &info) {
  fn_main();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
    exports.Set(Napi::String::New(env, "main"), Napi::Function::New(env, RunMain));
    return exports;
}
NODE_API_MODULE(addon, Init)