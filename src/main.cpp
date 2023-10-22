#define NOMINMAX
#include "main.h"
#include "utils/window/c_window.h"
#include "utils/ui/rmlui/c_rml_core.h"
#include "utils/ui/bgfx/c_bgfx_core.h"

void main()
{
    utils::c_window window;
    utils::ui::c_rml_core rml_core;
    utils::ui::c_bgfx_core bgfx_core;

    window.create();

    const auto size = window.get_size();
    const auto width = size.right;
    const auto height = size.bottom;

    bgfx_core.create(window.get_wnd(), width, height);
    rml_core.create(window.get_wnd(), width, height);
    
    window.on_update([&]() {
        bgfx_core.pre_render();

    	//bgfx_core.debug_render();
        rml_core.update();
        bgfx_core.post_render();
    });

    window.on_resize([&](RECT& size) {
        bgfx_core.resize(size.right, size.bottom);
        rml_core.resize(size.right, size.bottom);
    });

    window.on_wnd_proc([&](HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        utils::ui::c_rml_system_interface::wnd_proc(rml_core.get_ctx().get(), h_wnd, msg, w_param, l_param);
    });

    window.start();

    printf("shutting down\n");

    rml_core.destroy();
    window.destroy();
    bgfx_core.destroy();
}