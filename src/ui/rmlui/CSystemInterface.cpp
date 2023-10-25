#include "CSystemInterface.h"
#include "rml_helpers.h"
#include "window/CEvent.h"

using namespace window;
using namespace ui::helpers;

ui::CSystemInterface::CSystemInterface(SDL_Window* window) : m_window(window)
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

ui::CSystemInterface::~CSystemInterface()
{
	SDL_FreeCursor(m_cursor_default);
	SDL_FreeCursor(m_cursor_move);
	SDL_FreeCursor(m_cursor_pointer);
	SDL_FreeCursor(m_cursor_resize);
	SDL_FreeCursor(m_cursor_cross);
	SDL_FreeCursor(m_cursor_text);
	SDL_FreeCursor(m_cursor_unavailable);
}

double ui::CSystemInterface::GetElapsedTime()
{
	return (bx::getHPCounter() - m_start_time) / (double)bx::getHPFrequency();
}

void ui::CSystemInterface::SetMouseCursor(const Rml::String& cursor_name)
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

void ui::CSystemInterface::SetClipboardText(const Rml::String& text_utf8)
{
	SDL_SetClipboardText(text_utf8.c_str());
}

void ui::CSystemInterface::GetClipboardText(Rml::String& text)
{
	char* raw_text = SDL_GetClipboardText();
	text = Rml::String(raw_text);
	SDL_free(raw_text);
}

bool ui::CSystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message)
{
    std::stringstream str;
    switch (type)
    {
	    case Rml::Log::Type::LT_ERROR:
    		SPDLOG_ERROR("{}", message.c_str());
	        break;
	    case Rml::Log::Type::LT_DEBUG:
	    case Rml::Log::Type::LT_ASSERT:
    		SPDLOG_DEBUG("{}", message.c_str());
	        break;
	    case Rml::Log::Type::LT_WARNING:
    		SPDLOG_WARN("{}", message.c_str());
	        break;
	    case Rml::Log::Type::LT_INFO:
	    default:
    		SPDLOG_INFO("{}", message.c_str());
	        break;
    }

    return true;
}

bool ui::CSystemInterface::OnEvent(Rml::Context* context, CEvent* event)
{
	if (!context)
		return true;

	bool result = true;

	switch (event->m_type)
	{
	case SDL_MOUSEMOTION: {
		const auto ev = (CEventMouseMotion*)event;
		result = context->ProcessMouseMove(ev->m_x, ev->m_y, GetKeyModifierState(ev->m_key_mod));
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		const auto ev = (CEventMouseButton*)event;
		if(ev->m_down) result = context->ProcessMouseButtonDown(ConvertMouseButton(ev->m_btn), GetKeyModifierState(ev->m_key_mod));
		else result = context->ProcessMouseButtonUp(ConvertMouseButton(ev->m_btn), GetKeyModifierState(ev->m_key_mod));
		break;
	}
	case SDL_MOUSEWHEEL: {
		const auto ev = (CEventMouseWheel*)event;
		result = context->ProcessMouseWheel(ev->m_y, GetKeyModifierState(ev->m_key_mod));
		break;
	}
	case SDL_KEYDOWN: {
		const auto ev = (CEventKey*)event;
		if (ev->m_down)
		{
			result = context->ProcessKeyDown(ConvertKey(ev->m_key), GetKeyModifierState(ev->m_key_mod));
			if (ev->m_key == SDLK_RETURN || ev->m_key == SDLK_KP_ENTER)
				result &= context->ProcessTextInput('\n');
		}
		else result = context->ProcessKeyUp(ConvertKey(ev->m_key), GetKeyModifierState(ev->m_key_mod));
		break;
	}
	case SDL_TEXTINPUT: {
		const auto ev = (CEventTextInput*)event;
		result = context->ProcessTextInput(Rml::String(ev->m_data));
		break;
	}
	case SDL_WINDOWEVENT_LEAVE:
		context->ProcessMouseLeave();
		break;
	}

	return result;
}