#pragma once
#include "pch.h"
#include "ui/bgfx/CCoreBGFX.h"
#include "core/CLogger.h"
#include "api/api.h"

namespace window
{
    class CEvent;
    class CWindow
    {
    private:
        SDL_Window* m_window;
        std::atomic_bool m_running = false;
        std::atomic_bool m_ready = false;

        Eigen::Vector2i m_wnd_pos;
        Eigen::Vector2i m_wnd_size;

        std::unique_ptr<ui::CCoreBGFX> m_bgfx;
        core::CLogger m_logger{};

        App::t_rml_event_handler m_event_handler;
        App::t_generic_event_handler m_render_handler;
        App::t_generic_event_handler m_update_handler;
        App::t_generic_event_handler m_window_init_handler;
        App::t_generic_event_handler m_render_init_handler;
        
    public:
        CWindow(const Eigen::Vector4i& size);
        ~CWindow();

        void Start();
        void Loop();
        void RunApi();

        bool SendEvent(Rml::Element* target, const Rml::String& name, const Rml::Dictionary& parameters);
        void RegisterEventHandler(const App::t_rml_event_handler& handler) { m_event_handler = handler; };
        void RegisterRenderEventHandler(const App::t_generic_event_handler& handler) { m_render_handler = handler; };
        void RegisterUpdateEventHandler(const App::t_generic_event_handler& handler) { m_update_handler = handler; };
        void RegisterWindowInitEventHandler(const App::t_generic_event_handler& handler) { m_window_init_handler = handler; };
        void RegisterRenderInitEventHandler(const App::t_generic_event_handler& handler) { m_render_init_handler = handler; };

        bool IsRunning() const { return m_running; }
        ui::CCoreBGFX* GetBGFX() const { return m_bgfx.get(); }
        bool IsReady() const { return m_ready; }

        SDL_Window* GetWindowPtr() const { return m_window; }
    };
}
