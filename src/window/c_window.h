#pragma once
#include "main.h"

namespace window
{
	class c_window
	{
	private:
		typedef std::function<void()> update_cb_type;
		typedef std::function<void(HWND, UINT, WPARAM, LPARAM)> wnd_proc_cb_type;
		typedef std::function<void(Eigen::Vector2i&)> wnd_resize_cb_type;

		update_cb_type update_cb{};
		wnd_proc_cb_type wnd_proc_cb{};
		wnd_resize_cb_type wnd_resize_cb{};

		HWND h_wnd = nullptr;

		Eigen::Vector2i m_wnd_pos;
		Eigen::Vector2i m_wnd_size;
		
	public:
		void create(int x, int y, int width, int height);
		void destroy();

		void start();

		HWND get_wnd() const { return h_wnd; }
		Eigen::Vector2i get_size() const { return m_wnd_size; }

		void on_update(const update_cb_type& cb) { update_cb = cb; };
		void on_wnd_proc(const wnd_proc_cb_type& cb) { wnd_proc_cb = cb; };
		void on_resize(const wnd_resize_cb_type& cb) { wnd_resize_cb = cb; };

	private:
		static LRESULT WINAPI wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);
	};
}
