#pragma once
#include "main.h"

namespace ui
{
	class c_bgfx_core
	{
	private:
		int m_width = 0;
		int m_height = 0;


	public:
		void create(SDL_Window* window, int width, int height);
		void destroy();
		void resize(int width, int height);
		void pre_render();
		void debug_render();
		void post_render();
	};
}
