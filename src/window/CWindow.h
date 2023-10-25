#pragma once
#include "pch.h"

namespace window
{
	class CEvent;
	class CWindow
	{
	private:
		typedef std::function<void()> init_cb_type;
		typedef std::function<void()> uninit_cb_type;
		typedef std::function<void()> update_cb_type;
		typedef std::function<void()> render_cb_type;
		typedef std::function<void(Eigen::Vector2i&)> wnd_resize_cb_type;

		typedef std::function<void(CEvent*)> t_event_cb;

		init_cb_type m_init_cb{};
		uninit_cb_type m_uninit_cb{};
		update_cb_type m_update_cb{};
		render_cb_type m_render_cb{};
		wnd_resize_cb_type m_wnd_resize_cb{};
		t_event_cb m_event_cb{};

		SDL_Window* m_window;
		std::atomic_bool m_running;

		Eigen::Vector2i m_wnd_pos;
		Eigen::Vector2i m_wnd_size;
		
	public:
		void Create(int x, int y, int width, int height);
		void Destroy();

		void Start();

		SDL_Window* GetWindowPtr() const { return m_window; }
		Eigen::Vector2i GetSize() const { return m_wnd_size; }

		void OnUpdate(const update_cb_type& cb) { m_update_cb = cb; };
		void OnRender(const render_cb_type& cb) { m_render_cb = cb; };
		void OnResize(const wnd_resize_cb_type& cb) { m_wnd_resize_cb = cb; };
		void OnInit(const init_cb_type& cb) { m_init_cb = cb; };
		void OnUninit(const uninit_cb_type& cb) { m_uninit_cb = cb; };
		void OnEvent(const t_event_cb& cb) { m_event_cb = cb; };

	private:
		static int32_t RunApiThread(bx::Thread* self, void* userData);
	};
}
