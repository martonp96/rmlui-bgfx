#pragma once
#include "main.h"

namespace window
{
	class c_window
	{
	private:
		typedef std::function<void()> update_cb_type;
		typedef std::function<void(SDL_Event&)> key_event_cb_type;
		typedef std::function<void(Eigen::Vector2i&)> wnd_resize_cb_type;

		update_cb_type update_cb{};
		key_event_cb_type m_key_event_cb{};
		wnd_resize_cb_type wnd_resize_cb{};

		SDL_Window* m_window;
		bool m_running;

		Eigen::Vector2i m_wnd_pos;
		Eigen::Vector2i m_wnd_size;
		
	public:
		void create(int x, int y, int width, int height);
		void destroy();

		void start();

		SDL_Window* get_wnd() const { return m_window; }
		Eigen::Vector2i get_size() const { return m_wnd_size; }

		void on_update(const update_cb_type& cb) { update_cb = cb; };
		void on_key_event(const key_event_cb_type& cb) { m_key_event_cb = cb; };
		void on_resize(const wnd_resize_cb_type& cb) { wnd_resize_cb = cb; };

	private:
		void key_event(SDL_Event& ev);
	};
}
