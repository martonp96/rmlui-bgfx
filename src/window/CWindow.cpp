#include "CWindow.h"
#include "CEvent.h"

using namespace window;

static bx::DefaultAllocator g_allocator;
static bx::SpScUnboundedQueue g_api_thread_events(&g_allocator);

CWindow::CWindow(const Eigen::Vector4i& size)
{
    m_wnd_pos = { size[0], size[1] };
    m_wnd_size = { size[2], size[3] };

    const auto rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    if (rc != 0)
    {
        SPDLOG_ERROR("SDL error on init: {} {}", rc, SDL_GetError());
        return;
    }

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    m_window = SDL_CreateWindow("app_window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_wnd_size[0], m_wnd_size[1], SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        SPDLOG_ERROR("SDL error on create window: {}", SDL_GetError());
        return;
    }

    SDL_ShowWindow(m_window);

    m_event_handler = nullptr;
    m_render_handler = nullptr;
    m_update_handler = nullptr;

    m_ready = false;
}

CWindow::~CWindow()
{
    m_running = false;

    while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {}

    m_api_thread.shutdown();
    m_bgfx.reset();

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void CWindow::Start()
{
    m_running = true;
    bgfx::renderFrame();
    m_api_thread.init(ApiThread, this);
}

void CWindow::RunApi()
{
    m_bgfx = std::make_unique<ui::CCoreBGFX>(this, m_wnd_size);
    m_ready = true;

    if (m_render_init_handler)
        m_render_init_handler();

    while (m_running)
    {
        while (auto ev = (CEvent*)g_api_thread_events.pop())
        {
            switch (ev->m_type)
            {
            case SDL_QUIT:
                m_running = false;
                m_bgfx.reset();
                return;
            case SDL_WINDOWEVENT_RESIZED: {
                const auto event = (CEventResize*)ev;
                m_wnd_size = { event->m_width, event->m_height };
                m_bgfx->Resize(m_wnd_size);
                break;
            }
            default:
                m_bgfx->OnEvent(ev);
                break;
            }
            delete ev;
        }

        if (m_render_handler)
            m_render_handler();

        m_bgfx->Render();
    }
}

bool CWindow::SendEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible)
{
    if (m_event_handler)
        return m_event_handler(reinterpret_cast<rml::handle>(target), (int)id, name.c_str(), reinterpret_cast<rml::dict::handle>((void*)&parameters), interruptible);

    return true;
}

int32_t CWindow::ApiThread(bx::Thread* self, void* userData)
{
    static_cast<CWindow*>(userData)->RunApi();
    return 0;
}

void CWindow::Loop()
{
    if (!m_running)
        return;

    SDL_Event event;
    while (SDL_PollEvent(&event) && m_running)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            g_api_thread_events.push(new CEvent(SDL_QUIT));
            break;

        case SDL_WINDOWEVENT:
        {
            const SDL_WindowEvent& ev = event.window;
            switch (ev.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                g_api_thread_events.push(new CEventResize(ev.data1, ev.data2));
                break;
            case SDL_WINDOWEVENT_CLOSE:
                g_api_thread_events.push(new CEvent(SDL_QUIT));
                break;
            case SDL_WINDOWEVENT_LEAVE:
                g_api_thread_events.push(new CEvent(SDL_WINDOWEVENT_LEAVE));
                break;
            }
            break;
        }
        case SDL_MOUSEMOTION:
            g_api_thread_events.push(new CEventMouseMotion(event.motion.x, event.motion.y, SDL_GetModState()));
            break;
        case SDL_MOUSEBUTTONDOWN:
            g_api_thread_events.push(new CEventMouseButton(true, event.button.button, SDL_GetModState()));
            SDL_CaptureMouse(SDL_TRUE);
            break;
        case SDL_MOUSEBUTTONUP:
            SDL_CaptureMouse(SDL_FALSE);
            g_api_thread_events.push(new CEventMouseButton(false, event.button.button, SDL_GetModState()));
            break;
        case SDL_MOUSEWHEEL:
            g_api_thread_events.push(new CEventMouseWheel(float(-event.wheel.y), SDL_GetModState()));
            break;
        case SDL_KEYDOWN:
            g_api_thread_events.push(new CEventKey(true, event.key.keysym.sym, SDL_GetModState()));
            break;
        case SDL_KEYUP:
            g_api_thread_events.push(new CEventKey(false, event.key.keysym.sym, SDL_GetModState()));
            break;
        case SDL_TEXTINPUT:
            g_api_thread_events.push(new CEventTextInput(event.text.text));
            break;
        }
    }
    
    if (!m_running) return;

    bgfx::renderFrame();
}
