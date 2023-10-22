#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/StringUtilities.h>
#include <RmlUi/Core/SystemInterface.h>

#include "c_rml_system_interface.h"
#include "rml_helpers.h"

using namespace utils;

ui::c_rml_system_interface::c_rml_system_interface(HWND h_wnd_) : h_wnd(h_wnd_), start_time(std::chrono::system_clock::now())
{
	cursor_default = LoadCursor(nullptr, IDC_ARROW);
	cursor_move = LoadCursor(nullptr, IDC_SIZEALL);
	cursor_pointer = LoadCursor(nullptr, IDC_HAND);
	cursor_resize = LoadCursor(nullptr, IDC_SIZENWSE);
	cursor_cross = LoadCursor(nullptr, IDC_CROSS);
	cursor_text = LoadCursor(nullptr, IDC_IBEAM);
	cursor_unavailable = LoadCursor(nullptr, IDC_NO);

	LARGE_INTEGER time_ticks_per_second;
	QueryPerformanceFrequency(&time_ticks_per_second);
	QueryPerformanceCounter(&m_time_startup);

	m_time_frequency = 1.0 / (double)time_ticks_per_second.QuadPart;
}

double ui::c_rml_system_interface::GetElapsedTime()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	return double(counter.QuadPart - m_time_startup.QuadPart) * m_time_frequency;
}

void ui::c_rml_system_interface::SetMouseCursor(const Rml::String& cursor_name)
{
	if (!h_wnd) return;

	HCURSOR cursor_handle = nullptr;
	if (cursor_name.empty() || cursor_name == "arrow")
		cursor_handle = cursor_default;
	else if (cursor_name == "move")
		cursor_handle = cursor_move;
	else if (cursor_name == "pointer")
		cursor_handle = cursor_pointer;
	else if (cursor_name == "resize")
		cursor_handle = cursor_resize;
	else if (cursor_name == "cross")
		cursor_handle = cursor_cross;
	else if (cursor_name == "text")
		cursor_handle = cursor_text;
	else if (cursor_name == "unavailable")
		cursor_handle = cursor_unavailable;
	else if (cursor_name.find("rmlui-scroll") != std::string::npos)
		cursor_handle = cursor_move;

	if (cursor_handle)
	{
		SetCursor(cursor_handle);
		SetClassLongPtrA(h_wnd, GCLP_HCURSOR, (LONG_PTR)cursor_handle);
	}
}

void ui::c_rml_system_interface::SetClipboardText(const Rml::String& text_utf8)
{
	if (!h_wnd || !OpenClipboard(h_wnd)) return;
	
	EmptyClipboard();

	const std::wstring text = helpers::ConvertToUTF16(text_utf8);
	const size_t size = sizeof(wchar_t) * (text.size() + 1);

	HGLOBAL clipboard_data = GlobalAlloc(GMEM_FIXED, size);
	memcpy(clipboard_data, text.data(), size);

	if (SetClipboardData(CF_UNICODETEXT, clipboard_data) == nullptr)
	{
		CloseClipboard();
		GlobalFree(clipboard_data);
	}
	else
		CloseClipboard();
}

void ui::c_rml_system_interface::GetClipboardText(Rml::String& text)
{
	if (!h_wnd || !OpenClipboard(h_wnd)) return;

	HANDLE clipboard_data = GetClipboardData(CF_UNICODETEXT);
	if (clipboard_data == nullptr)
	{
		CloseClipboard();
		return;
	}

	const wchar_t* clipboard_text = (const wchar_t*)GlobalLock(clipboard_data);
	if (clipboard_text)
		text = helpers::ConvertToUTF8(clipboard_text);
	GlobalUnlock(clipboard_data);

	CloseClipboard();
}

void ui::c_rml_system_interface::ActivateKeyboard(Rml::Vector2f caret_position, float line_height)
{
	if (!h_wnd) return;
	if (HIMC himc = ImmGetContext(h_wnd))
	{
		COMPOSITIONFORM comp = {};
		comp.ptCurrentPos.x = (LONG)caret_position.x;
		comp.ptCurrentPos.y = (LONG)caret_position.y;
		comp.dwStyle = CFS_FORCE_POSITION;
		ImmSetCompositionWindow(himc, &comp);

		CANDIDATEFORM cand = {};
		cand.dwStyle = CFS_CANDIDATEPOS;
		cand.ptCurrentPos.x = (LONG)caret_position.x;
		cand.ptCurrentPos.y = (LONG)caret_position.y;
		ImmSetCandidateWindow(himc, &cand);

		ImmReleaseContext(h_wnd, himc);
	}
}

bool ui::c_rml_system_interface::LogMessage(Rml::Log::Type type, const Rml::String& message)
{
    std::stringstream str;
    switch (type)
    {
    case Rml::Log::Type::LT_ERROR:
        str << "[ERROR]";
        break;
    case Rml::Log::Type::LT_ASSERT:
        str << "[ASSERT]";
        break;
    case Rml::Log::Type::LT_WARNING:
        str << "[WARNING]";
        break;
    case Rml::Log::Type::LT_INFO:
        str << "[INFO]";
        break;
    case Rml::Log::Type::LT_DEBUG:
        str << "[DEBUG]";
        break;
    default:
        str << "[DEFAULT]";
    }

    str << " " << message.c_str();

    printf("%s\n", str.str().c_str());

    return true;
}

bool ui::c_rml_system_interface::wnd_proc(Rml::Context* context, HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (!context)
		return true;

	static bool tracking_mouse_leave = false;

	bool result = true;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		result = context->ProcessMouseButtonDown(0, helpers::GetKeyModifierState());
		SetCapture(window_handle);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		result = context->ProcessMouseButtonUp(0, helpers::GetKeyModifierState());
		break;
	case WM_RBUTTONDOWN: result = context->ProcessMouseButtonDown(1, helpers::GetKeyModifierState()); break;
	case WM_RBUTTONUP: result = context->ProcessMouseButtonUp(1, helpers::GetKeyModifierState()); break;
	case WM_MBUTTONDOWN: result = context->ProcessMouseButtonDown(2, helpers::GetKeyModifierState()); break;
	case WM_MBUTTONUP: result = context->ProcessMouseButtonUp(2, helpers::GetKeyModifierState()); break;
	case WM_MOUSEMOVE:
		result = context->ProcessMouseMove(static_cast<int>((short)LOWORD(l_param)), static_cast<int>((short)HIWORD(l_param)),
			helpers::GetKeyModifierState());

		if (!tracking_mouse_leave)
		{
			TRACKMOUSEEVENT tme = {};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = window_handle;
			tracking_mouse_leave = TrackMouseEvent(&tme);
		}
		break;
	case WM_MOUSEWHEEL:
		result = context->ProcessMouseWheel(static_cast<float>((short)HIWORD(w_param)) / static_cast<float>(-WHEEL_DELTA),
			helpers::GetKeyModifierState());
		break;
	case WM_MOUSELEAVE:
		result = context->ProcessMouseLeave();
		tracking_mouse_leave = false;
		break;
	case WM_KEYDOWN: result = context->ProcessKeyDown(helpers::ConvertKey((int)w_param), helpers::GetKeyModifierState()); break;
	case WM_KEYUP: result = context->ProcessKeyUp(helpers::ConvertKey((int)w_param), helpers::GetKeyModifierState()); break;
	case WM_CHAR:
	{
		static wchar_t first_u16_code_unit = 0;

		const wchar_t c = (wchar_t)w_param;
		Rml::Character character = (Rml::Character)c;

		// Windows sends two-wide characters as two messages.
		if (c >= 0xD800 && c < 0xDC00)
		{
			// First 16-bit code unit of a two-wide character.
			first_u16_code_unit = c;
		}
		else
		{
			if (c >= 0xDC00 && c < 0xE000 && first_u16_code_unit != 0)
			{
				// Second 16-bit code unit of a two-wide character.
				Rml::String utf8 = helpers::ConvertToUTF8(std::wstring{first_u16_code_unit, c});
				character = Rml::StringUtilities::ToCharacter(utf8.data());
			}
			else if (c == '\r')
			{
				// Windows sends new-lines as carriage returns, convert to endlines.
				character = (Rml::Character)'\n';
			}

			first_u16_code_unit = 0;

			// Only send through printable characters.
			if (((char32_t)character >= 32 || character == (Rml::Character)'\n') && character != (Rml::Character)127)
				result = context->ProcessTextInput(character);
		}
	}
	break;
	default: break;
	}

	return result;
}