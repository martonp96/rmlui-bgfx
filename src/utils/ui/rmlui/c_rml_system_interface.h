#pragma once
#include "main.h"
#include <RmlUi/Core.h>

namespace utils::ui
{
	class c_rml_system_interface : public Rml::SystemInterface
	{
	private:
		HWND h_wnd;
		int64_t m_start_time;

		HCURSOR cursor_default = nullptr;
		HCURSOR cursor_move = nullptr;
		HCURSOR cursor_pointer = nullptr;
		HCURSOR cursor_resize = nullptr;
		HCURSOR cursor_cross = nullptr;
		HCURSOR cursor_text = nullptr;
		HCURSOR cursor_unavailable = nullptr;

	public:
		c_rml_system_interface(HWND h_wnd_);

		double GetElapsedTime() override;
		void SetMouseCursor(const Rml::String& cursor_name) override;
		void SetClipboardText(const Rml::String& text_utf8) override;
		void GetClipboardText(Rml::String& text) override;
		void ActivateKeyboard(Rml::Vector2f caret_position, float line_height) override;

		bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

		static bool wnd_proc(Rml::Context* context, HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
	};
}