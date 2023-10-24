#pragma once

namespace window
{
	class CEvent
	{
	public:
		CEvent(uint16_t type) : m_type(type) {}

		uint16_t m_type;
	};

	class CEventMouseMotion : public CEvent
	{
	public:
		CEventMouseMotion(int32_t x, int32_t y, SDL_Keymod key_mod) : CEvent(SDL_MOUSEMOTION), m_x(x), m_y(y), m_key_mod(key_mod) {}

		SDL_Keymod m_key_mod;
		int32_t m_x, m_y;
	};

	class CEventMouseButton : public CEvent
	{
	public:
		CEventMouseButton(bool down, uint8_t btn, SDL_Keymod key_mod) : CEvent(SDL_MOUSEBUTTONDOWN), m_down(down), m_btn(btn), m_key_mod(key_mod) {}

		SDL_Keymod m_key_mod;
		bool m_down;
		uint8_t m_btn;
	};

	class CEventKey : public CEvent
	{
	public:
		CEventKey(bool down, int32_t key, SDL_Keymod key_mod) : CEvent(SDL_KEYDOWN), m_down(down), m_key(key), m_key_mod(key_mod) {}

		SDL_Keymod m_key_mod;
		bool m_down;
		int32_t m_key;
	};

	class CEventMouseWheel : public CEvent
	{
	public:
		CEventMouseWheel(float y, SDL_Keymod key_mod) : CEvent(SDL_MOUSEWHEEL), m_y(y), m_key_mod(key_mod) {}

		SDL_Keymod m_key_mod;
		float m_y;
	};

	class CEventTextInput : public CEvent
	{
	public:
		CEventTextInput(const char* data) : CEvent(SDL_TEXTINPUT)
		{
			std::memcpy(m_data, data, 32);
		}

		char m_data[32];
	};

	class CEventResize : public CEvent
	{
	public:
		CEventResize(int32_t width, int32_t height) : CEvent(SDL_WINDOWEVENT_RESIZED), m_width(width), m_height(height) {}

		int32_t m_width, m_height;
	};
}