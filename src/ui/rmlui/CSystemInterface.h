#pragma once
#include "pch.h"

namespace ui
{
    class CSystemInterface : public Rml::SystemInterface
    {
    private:
        int64_t m_start_time;

        SDL_Window* m_window = nullptr;

        SDL_Cursor* m_cursor_default = nullptr;
        SDL_Cursor* m_cursor_move = nullptr;
        SDL_Cursor* m_cursor_pointer = nullptr;
        SDL_Cursor* m_cursor_resize = nullptr;
        SDL_Cursor* m_cursor_cross = nullptr;
        SDL_Cursor* m_cursor_text = nullptr;
        SDL_Cursor* m_cursor_unavailable = nullptr;

    public:
        CSystemInterface(SDL_Window* window);
        ~CSystemInterface();

        double GetElapsedTime() override;
        void SetMouseCursor(const Rml::String& cursor_name) override;
        void SetClipboardText(const Rml::String& text_utf8) override;
        void GetClipboardText(Rml::String& text) override;

        bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

        static bool OnEvent(Rml::Context* context, window::CEvent* ev);
    };
}