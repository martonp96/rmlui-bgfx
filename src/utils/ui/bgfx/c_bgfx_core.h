#pragma once
#include "main.h"

namespace utils::ui
{
	class c_bgfx_core
	{
	private:
		int m_width = 0;
		int m_height = 0;


	public:
		void create(HWND h_wnd, int width, int height);
		void destroy();
		void resize(int width, int height);
		void pre_render();
		void debug_render();
		void post_render();
	};
}
