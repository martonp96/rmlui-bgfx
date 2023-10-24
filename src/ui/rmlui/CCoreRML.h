#pragma once
#include "main.h"

#include "CRenderInterface.h"
#include "CSystemInterface.h"


namespace ui
{
	class CCoreRML
	{
	private:
		typedef std::unique_ptr<Rml::Context, std::function<void(Rml::Context*)>> rml_ctx_type;
		rml_ctx_type m_ctx;
		
		std::unique_ptr<CRenderInterface> m_render_interface;
		std::unique_ptr<CSystemInterface> m_system_interface;

	public:
		void Create(SDL_Window* window, int width, int height);
		void Resize(int width, int height);
		void Destroy();
		void Render();

		rml_ctx_type& GetContext() { return m_ctx; }
	};
}
