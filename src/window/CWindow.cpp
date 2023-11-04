#include "CWindow.h"
#include "CEvent.h"

using namespace window;

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
    
    m_bgfx = std::make_unique<ui::CCoreBGFX>(this, m_wnd_size);
    m_ready = true;
}

CWindow::~CWindow()
{
    m_running = false;

    m_bgfx.reset();

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void CWindow::Start()
{
    m_running = true;

    if (m_render_init_handler)
        m_render_init_handler();
}

bool CWindow::SendEvent(Rml::Element* target, const Rml::String& name, const Rml::Dictionary& parameters)
{
    if (m_event_handler)
    {
        std::unordered_map<std::string, t_api_variant> map;

        for(auto& param : parameters)
        {
            map[param.first] = api::FromRmlVariant(param.second);
            //SPDLOG_INFO("param {} type {}", param.first, map[param.first].index());
        }

        return m_event_handler(api::GetOrCreateElement(target), name.c_str(), map);
    }

    return true;
}

void CWindow::Loop()
{
    if (!m_running)
        return;

    SDL_Event event;
    while (SDL_PollEvent(&event) && m_running)
    {
        CEvent* window_event = nullptr;
        switch (event.type)
        {
        case SDL_QUIT:
            window_event = new CEvent(SDL_QUIT);
            break;

        case SDL_WINDOWEVENT:
        {
            const SDL_WindowEvent& ev = event.window;
            switch (ev.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                window_event = new CEventResize(ev.data1, ev.data2);
                m_wnd_size = { ev.data1, ev.data2 };
                m_bgfx->Resize(m_wnd_size);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                window_event = new CEvent(SDL_QUIT);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                window_event = new CEvent(SDL_WINDOWEVENT_LEAVE);
                break;
            }
            break;
        }
        case SDL_MOUSEMOTION:
            window_event = new CEventMouseMotion(event.motion.x, event.motion.y, SDL_GetModState());
            break;
        case SDL_MOUSEBUTTONDOWN:
            window_event = new CEventMouseButton(true, event.button.button, SDL_GetModState());
            SDL_CaptureMouse(SDL_TRUE);
            break;
        case SDL_MOUSEBUTTONUP:
            SDL_CaptureMouse(SDL_FALSE);
            window_event = new CEventMouseButton(false, event.button.button, SDL_GetModState());
            break;
        case SDL_MOUSEWHEEL:
            window_event = new CEventMouseWheel(float(-event.wheel.y), SDL_GetModState());
            break;
        case SDL_KEYDOWN:
            window_event = new CEventKey(true, event.key.keysym.sym, SDL_GetModState());
            break;
        case SDL_KEYUP:
            window_event = new CEventKey(false, event.key.keysym.sym, SDL_GetModState());
            break;
        case SDL_TEXTINPUT:
            window_event = new CEventTextInput(event.text.text);
            break;
        }

        if (window_event)
        {
            m_bgfx->OnEvent(window_event);

            if (window_event->m_type == SDL_QUIT)
            {
                m_running = false;
                m_bgfx.reset();
                return;
            }

            delete window_event;
        }
    }
    
    if (m_render_handler)
        m_render_handler();

    m_bgfx->Render();
}
