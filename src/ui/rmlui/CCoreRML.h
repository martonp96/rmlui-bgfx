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
		void create(SDL_Window* window, int width, int height);
		void resize(int width, int height);
		void destroy();
		void update();

		rml_ctx_type& get_ctx() { return m_ctx; }
	};
}
