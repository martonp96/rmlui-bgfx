#include "CCoreBGFX.h"

void ui::CCoreBGFX::Create(SDL_Window* window, int width, int height)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        return;
    }

    bgfx::Init init;
    init.type = bgfx::RendererType::Direct3D11;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = wmi.info.x11.display;
    init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    init.platformData.ndt = nullptr;
    init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    init.platformData.ndt = nullptr;
    init.platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
    init.platformData.ndt = wmi.info.vivante.display;
    init.platformData.nwh = wmi.info.vivante.window;
#endif

    bgfx::init(init);

    m_width = width;
    m_height = height;
    
    // Enable debug text.
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Set view 0 clear state.
    bgfx::setViewName(0, "UI Layer");
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setViewMode(0, bgfx::ViewMode::Sequential);
    bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
    bgfx::setViewScissor(0, 0, 0, 0, 0);
}

void ui::CCoreBGFX::Destroy()
{
    bgfx::shutdown();
}

void ui::CCoreBGFX::Resize(int width, int height)
{
    m_width = width;
    m_height = height;

    bgfx::reset(width, height);
    bgfx::setViewRect(0, 0, 0, width, height);
}

void ui::CCoreBGFX::PreRender()
{
    bgfx::touch(0);
}

void ui::CCoreBGFX::PostRender()
{
    bgfx::frame();
}

void ui::CCoreBGFX::DebugRender()
{
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();

    const bgfx::Stats* stats = bgfx::getStats();

    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    
    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

    bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters"
        , stats->width
        , stats->height
        , stats->textWidth
        , stats->textHeight
    );
    
}
