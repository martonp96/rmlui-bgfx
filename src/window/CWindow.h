#pragma once
#include "pch.h"
#include "ui/bgfx/CCoreBGFX.h"
#include "core/CLogger.h"

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
        
    public:
        CWindow(const Eigen::Vector4i& size);
        ~CWindow();

        void Loop();
        void RunApi();

        bool IsRunning() const { return m_running; }
        ui::CCoreBGFX* GetBGFX() const { return m_bgfx.get(); }
        bool IsReady() const { return m_ready; }

    private:
        static int32_t ApiThread(bx::Thread* self, void* userData);
    };
}
