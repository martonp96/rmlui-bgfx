#include "c_rml_system_interface.h"
#include "rml_helpers.h"

ui::c_rml_system_interface::c_rml_system_interface(SDL_Window* window) : m_window(window)
{
	m_cursor_default = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_cursor_move = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	m_cursor_pointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	m_cursor_resize = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	m_cursor_cross = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	m_cursor_text = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	m_cursor_unavailable = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

	m_start_time = bx::getHPCounter();
}

ui::c_rml_system_interface::~c_rml_system_interface()
{
	SDL_FreeCursor(m_cursor_default);
	SDL_FreeCursor(m_cursor_move);
	SDL_FreeCursor(m_cursor_pointer);
	SDL_FreeCursor(m_cursor_resize);
	SDL_FreeCursor(m_cursor_cross);
	SDL_FreeCursor(m_cursor_text);
	SDL_FreeCursor(m_cursor_unavailable);
}

double ui::c_rml_system_interface::GetElapsedTime()
{
	return (bx::getHPCounter() - m_start_time) / (double)bx::getHPFrequency();
}

void ui::c_rml_system_interface::SetMouseCursor(const Rml::String& cursor_name)
{
	SDL_Cursor* cursor = nullptr;

	if (cursor_name.empty() || cursor_name == "arrow")
		cursor = m_cursor_default;
	else if (cursor_name == "move")
		cursor = m_cursor_move;
	else if (cursor_name == "pointer")
		cursor = m_cursor_pointer;
	else if (cursor_name == "resize")
		cursor = m_cursor_resize;
	else if (cursor_name == "cross")
		cursor = m_cursor_cross;
	else if (cursor_name == "text")
		cursor = m_cursor_text;
	else if (cursor_name == "unavailable")
		cursor = m_cursor_unavailable;
	else if (Rml::StringUtilities::StartsWith(cursor_name, "rmlui-scroll"))
		cursor = m_cursor_move;

	if (cursor)
		SDL_SetCursor(cursor);
}

void ui::c_rml_system_interface::SetClipboardText(const Rml::String& text_utf8)
{
	SDL_SetClipboardText(text_utf8.c_str());
}

void ui::c_rml_system_interface::GetClipboardText(Rml::String& text)
{
	char* raw_text = SDL_GetClipboardText();
	text = Rml::String(raw_text);
	SDL_free(raw_text);
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

using namespace ui::helpers;

bool ui::c_rml_system_interface::wnd_proc(Rml::Context* context, SDL_Event& ev)
{
	if (!context)
		return true;

	bool result = true;

	switch (ev.type)
	{
	case SDL_MOUSEMOTION: result = context->ProcessMouseMove(ev.motion.x, ev.motion.y, GetKeyModifierState()); break;
	case SDL_MOUSEBUTTONDOWN:
		result = context->ProcessMouseButtonDown(ConvertMouseButton(ev.button.button), GetKeyModifierState());
		SDL_CaptureMouse(SDL_TRUE);
		break;
	case SDL_MOUSEBUTTONUP:
		SDL_CaptureMouse(SDL_FALSE);
		result = context->ProcessMouseButtonUp(ConvertMouseButton(ev.button.button), GetKeyModifierState());
		break;
	case SDL_MOUSEWHEEL: result = context->ProcessMouseWheel(float(-ev.wheel.y), GetKeyModifierState()); break;
	case SDL_KEYDOWN:
		result = context->ProcessKeyDown(ConvertKey(ev.key.keysym.sym), GetKeyModifierState());
		if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER)
			result &= context->ProcessTextInput('\n');
		break;
	case SDL_KEYUP: result = context->ProcessKeyUp(ConvertKey(ev.key.keysym.sym), GetKeyModifierState()); break;
	case SDL_TEXTINPUT: result = context->ProcessTextInput(Rml::String(&ev.text.text[0])); break;
	case SDL_WINDOWEVENT:
	{
		switch (ev.window.event)
		{
		case SDL_WINDOWEVENT_LEAVE: context->ProcessMouseLeave(); break;
		}
	}
	break;
	default: break;
	}

	return result;
}