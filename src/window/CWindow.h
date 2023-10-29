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
        std::atomic_bool m_running;
        std::atomic_bool m_ready;

        Eigen::Vector2i m_wnd_pos;
        Eigen::Vector2i m_wnd_size;

        bx::Thread m_api_thread;

        std::unique_ptr<ui::CCoreBGFX> m_bgfx;
        core::CLogger m_logger{};

        app::t_rml_event_handler m_event_handler;
        app::t_generic_event_handler m_render_handler;
        app::t_generic_event_handler m_update_handler;
        app::t_generic_event_handler m_window_init_handler;
        app::t_generic_event_handler m_render_init_handler;
        
    public:
        CWindow(const Eigen::Vector4i& size);
        ~CWindow();

        void Start();
        void Loop();
        void RunApi();

        bool SendEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible);
        void RegisterEventHandler(app::t_rml_event_handler handler) { m_event_handler = handler; };
        void RegisterRenderEventHandler(app::t_generic_event_handler handler) { m_render_handler = handler; };
        void RegisterUpdateEventHandler(app::t_generic_event_handler handler) { m_update_handler = handler; };
        void RegisterWindowInitEventHandler(app::t_generic_event_handler handler) { m_window_init_handler = handler; };
        void RegisterRenderInitEventHandler(app::t_generic_event_handler handler) { m_render_init_handler = handler; };

        bool IsRunning() const { return m_running; }
        ui::CCoreBGFX* GetBGFX() const { return m_bgfx.get(); }
        bool IsReady() const { return m_ready; }

        SDL_Window* GetWindowPtr() const { return m_window; }

    private:
        static int32_t ApiThread(bx::Thread* self, void* userData);
    };
}
