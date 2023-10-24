#define STB_IMAGE_IMPLEMENTATION
#include "main.h"
#include "window/CWindow.h"
#include "ui/rmlui/CCoreRML.h"
#include "ui/bgfx/CCoreBGFX.h"

int main()
{
    window::CWindow window;
    ui::CCoreRML rml_core;
    ui::CCoreBGFX bgfx_core;

    window.Create(200, 200, 1800, 890);

    const auto size = window.GetSize();
    const auto width = size[0];
    const auto height = size[1];

    bgfx_core.Create(window.GetWindowPtr(), width, height);
    rml_core.create(window.GetWindowPtr(), width, height);
    
    window.OnUpdate([&]() {
        bgfx_core.PreRender();
    	//bgfx_core.DebugRender();
        rml_core.update();
        bgfx_core.PostRender();
    });

    window.OnResize([&](Eigen::Vector2i& size) {
        bgfx_core.Resize(size[0], size[1]);
        rml_core.resize(size[0], size[1]);
    });

    window.OnKeyEvent([&](SDL_Event& ev) {
        ui::CSystemInterface::OnEvent(rml_core.get_ctx().get(), ev);
    });

    window.Start();

    printf("shutting down\n");

    rml_core.destroy();
    window.Destroy();
    bgfx_core.Destroy();

    return 0;
}