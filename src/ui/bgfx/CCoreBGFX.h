#pragma once
#include "pch.h"

namespace ui
{
	class CCoreBGFX
	{
	private:
		int m_width = 0;
		int m_height = 0;


	public:
		void Create(SDL_Window* window, int width, int height);
		void Destroy();
		void Resize(int width, int height);
		void PreRender();
		void DebugRender();
		void PostRender();
	};
}
