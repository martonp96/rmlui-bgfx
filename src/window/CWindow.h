#pragma once
#include "main.h"

namespace window
{
	class CWindow
	{
	private:
		typedef std::function<void()> update_cb_type;
		typedef std::function<void(SDL_Event&)> key_event_cb_type;
		typedef std::function<void(Eigen::Vector2i&)> wnd_resize_cb_type;

		update_cb_type m_update_cb{};
		key_event_cb_type m_key_event_cb{};
		wnd_resize_cb_type m_wnd_resize_cb{};

		SDL_Window* m_window;
		bool m_running;

		Eigen::Vector2i m_wnd_pos;
		Eigen::Vector2i m_wnd_size;
		
	public:
		void Create(int x, int y, int width, int height);
		void Destroy();

		void Start();

		SDL_Window* GetWindowPtr() const { return m_window; }
		Eigen::Vector2i GetSize() const { return m_wnd_size; }

		void OnUpdate(const update_cb_type& cb) { m_update_cb = cb; };
		void OnKeyEvent(const key_event_cb_type& cb) { m_key_event_cb = cb; };
		void OnResize(const wnd_resize_cb_type& cb) { m_wnd_resize_cb = cb; };
	};
}
