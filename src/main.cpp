#define STB_IMAGE_IMPLEMENTATION
#include "pch.h"
#include "window/CWindow.h"
#include "ui/rmlui/CCoreRML.h"
#include "ui/bgfx/CCoreBGFX.h"

int main()
{
    window::CWindow window;
    ui::CCoreRML rml_core;
    ui::CCoreBGFX bgfx_core;
    core::CLogger logger;

    window.Create(200, 200, 1800, 890);

    const auto size = window.GetSize();
    const auto width = size[0];
    const auto height = size[1];

    window.OnInit([&]() {
        bgfx_core.Create(window.GetWindowPtr(), width, height);
        rml_core.Create(window.GetWindowPtr(), width, height);
    });

    window.OnUninit([&]() {
        rml_core.Destroy();
        bgfx_core.Destroy();
    });
    
    window.OnRender([&]() {
        bgfx_core.PreRender();
        rml_core.Render();
        bgfx_core.PostRender();
    });

    window.OnResize([&](Eigen::Vector2i& size) {
        bgfx_core.Resize(size[0], size[1]);
        rml_core.Resize(size[0], size[1]);
    });

    window.OnEvent([&](window::CEvent* ev) {
        ui::CSystemInterface::OnEvent(rml_core.GetContext().get(), ev);
    });

    window.Start();
    window.Destroy();

    SPDLOG_INFO("Shutting down");

    return 0;
}