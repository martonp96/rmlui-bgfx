#include "c_window.h"

using namespace window;

void c_window::create(int x, int y, int width, int height)
{
    m_wnd_pos = { x, y };
    m_wnd_size = { width, height };

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
        return;

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    m_window = SDL_CreateWindow("app_window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on create window: %s\n", SDL_GetError());
        return;
    }

    m_running = true;
    //FreeConsole();
}

void c_window::destroy()
{
    m_running = false;

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void c_window::start()
{
    SDL_Event event;

    while(m_running)
    {
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
                    if (wnd_resize_cb)
                        wnd_resize_cb(m_wnd_size);

                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    m_running = false;
                    break;
                }
                break;
            }
            default:
                key_event(event);
                break;
            }
        }

        if (update_cb)
            update_cb();
    }
}

void c_window::key_event(SDL_Event& ev)
{
    if (m_key_event_cb)
        m_key_event_cb(ev);
}