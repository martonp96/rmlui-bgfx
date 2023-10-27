#pragma once
#include "pch.h"
#include "ui/rmlui/CCoreRML.h"
#include "window/CEvent.h"

namespace ui
{
    class CCoreBGFX
    {
    private:
        Eigen::Vector2i m_size;

        std::unique_ptr<ui::CCoreRML> m_rml;
    public:
        CCoreBGFX(SDL_Window* window, const Eigen::Vector2i& size);
        ~CCoreBGFX();

        void Resize(const Eigen::Vector2i& size);
        void PreRender();
        void DebugRender();
        void PostRender();
        void Render();
        bool OnEvent(window::CEvent* event);

        ui::CCoreRML* GetRML() const { return m_rml.get(); }
    };
}
