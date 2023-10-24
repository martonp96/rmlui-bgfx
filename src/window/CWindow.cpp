#include "CWindow.h"

using namespace window;

void CWindow::Create(int x, int y, int width, int height)
{
    m_wnd_pos = { x, y };
    m_wnd_size = { width, height };

    auto rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    if (rc != 0)
    {
        printf("SDL error on init: %d %s\n", rc, SDL_GetError());
        return;
    }

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    m_window = SDL_CreateWindow("app_window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        printf("SDL error on create window: %s\n", SDL_GetError());
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
        if (user_data->m_update_cb)
            user_data->m_update_cb();
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
        bgfx::renderFrame();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                m_running = false;
                break;

            case SDL_WINDOWEVENT:
            {
                const SDL_WindowEvent& ev = event.window;
                switch (ev.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:

                    m_wnd_size[0] = ev.data1;
                    m_wnd_size[1] = ev.data2;
                    if (m_wnd_resize_cb)
                        m_wnd_resize_cb(m_wnd_size);

                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    m_running = false;
                    break;
                }
                break;
            }
            default:
                if (m_key_event_cb)
                    m_key_event_cb(event);
                break;
            }
        }
    }

    api_thread.shutdown();
}