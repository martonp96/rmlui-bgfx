#include "api/CCoreAPI.h"

int main()
{
    auto wnd = window::api::create(1800, 890);

    bool initialized = false;
    while (window::api::is_running(wnd))
    {
        if(!initialized && window::api::is_ready(wnd))
        {
            initialized = true;
            auto doc = window::api::load_document(wnd, "main.rml");
            rml::doc::api::set_visible(doc, true);
        }
        window::api::run_loop(wnd);
    }

    window::api::destroy(wnd);

    return 0;
}