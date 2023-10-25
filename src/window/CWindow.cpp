#include "CWindow.h"
#include "CEvent.h"

using namespace window;

static bx::DefaultAllocator g_allocator;
static bx::SpScUnboundedQueue g_api_thread_events(&g_allocator);

void CWindow::Create(int x, int y, int width, int height)
{
    m_wnd_pos = { x, y };
    m_wnd_size = { width, height };

    const auto rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    if (rc != 0)
    {
        SPDLOG_ERROR("SDL error on init: {} {}", rc, SDL_GetError());
        return;
    }

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    m_window = SDL_CreateWindow("app_window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        SPDLOG_ERROR("SDL error on create window: {}", SDL_GetError());
        return;
    }

    SDL_ShowWindow(m_window);

    m_running = true;
    //FreeConsole();
}

void CWindow::Destroy()
{
    m_running = false;

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

int32_t CWindow::RunApiThread(bx::Thread* self, void* userData)
{
    const auto user_data = (CWindow*)userData;
    if(user_data->m_init_cb)
        user_data->m_init_cb();

    while (user_data->m_running) 
    {
        while (auto ev = (CEvent*)g_api_thread_events.pop())
        {
            switch (ev->m_type)
            {
            case SDL_QUIT:
                user_data->m_running = false;
                if (user_data->m_uninit_cb)
                    user_data->m_uninit_cb();
                return 0;
            case SDL_WINDOWEVENT_RESIZED: {
                const auto event = (CEventResize*)ev;
                user_data->m_wnd_size = { event->m_width, event->m_height };
                if (user_data->m_wnd_resize_cb)
                    user_data->m_wnd_resize_cb(user_data->m_wnd_size);
                break;
            }
            default:
                if (user_data->m_event_cb)
                    user_data->m_event_cb(ev);
                break;
            }
            delete ev;
        }

        if (user_data->m_render_cb)
            user_data->m_render_cb();
    }

    return 0;
}

void CWindow::Start()
{
    bgfx::renderFrame();

    bx::Thread api_thread;
    api_thread.init(RunApiThread, this);

    while(m_running)
    {
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

        if (!m_running) break;

        if (m_update_cb)
            m_update_cb();

        bgfx::renderFrame();
    }

    while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {}

    api_thread.shutdown();
}