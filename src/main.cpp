#define STB_IMAGE_IMPLEMENTATION
#include "main.h"
#include "window/c_window.h"
#include "ui/rmlui/c_rml_core.h"
#include "ui/bgfx/c_bgfx_core.h"

int main()
{
    window::c_window window;
    ui::c_rml_core rml_core;
    ui::c_bgfx_core bgfx_core;

    window.create(200, 200, 1800, 890);

    const auto size = window.get_size();
    const auto width = size[0];
    const auto height = size[1];

    bgfx_core.create(window.get_wnd(), width, height);
    rml_core.create(window.get_wnd(), width, height);
    
    window.on_update([&]() {
        bgfx_core.pre_render();
    	//bgfx_core.debug_render();
        rml_core.update();
        bgfx_core.post_render();
    });

    window.on_resize([&](Eigen::Vector2i& size) {
        bgfx_core.resize(size[0], size[1]);
        rml_core.resize(size[0], size[1]);
    });

    window.on_wnd_proc([&](HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        ui::c_rml_system_interface::wnd_proc(rml_core.get_ctx().get(), h_wnd, msg, w_param, l_param);
    });

    window.start();

    printf("shutting down\n");

    rml_core.destroy();
    window.destroy();
    bgfx_core.destroy();

    return 0;
}