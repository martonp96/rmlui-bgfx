#pragma once
#include "main.h"

namespace utils
{
	class c_window
	{
	private:
		typedef std::function<void()> update_cb_type;
		typedef std::function<void(HWND, UINT, WPARAM, LPARAM)> wnd_proc_cb_type;
		typedef std::function<void(RECT&)> wnd_resize_cb_type;

		update_cb_type update_cb{};
		wnd_proc_cb_type wnd_proc_cb{};
		wnd_resize_cb_type wnd_resize_cb{};

		HWND h_wnd = nullptr;
		RECT wnd_size = { 200, 200, 1800, 890 };

	public:
		void create();
		void destroy();

		void start();

		HWND get_wnd() const { return h_wnd; }
		RECT get_size() const { return wnd_size; }

		void on_update(const update_cb_type& cb) { update_cb = cb; };
		void on_wnd_proc(const wnd_proc_cb_type& cb) { wnd_proc_cb = cb; };
		void on_resize(const wnd_resize_cb_type& cb) { wnd_resize_cb = cb; };

	private:
		static LRESULT WINAPI wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);
	};
}
