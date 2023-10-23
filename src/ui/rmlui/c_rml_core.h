#pragma once
#include "main.h"

#include "c_rml_render_interface.h"
#include "c_rml_system_interface.h"


namespace ui
{
	class c_rml_core
	{
	private:
		typedef std::unique_ptr<Rml::Context, std::function<void(Rml::Context*)>> rml_ctx_type;
		rml_ctx_type ctx;
		
		std::unique_ptr<c_rml_render_interface> render_interface;
		std::unique_ptr<c_rml_system_interface> system_interface;


	public:
		void create(SDL_Window* window, int width, int height);
		void resize(int width, int height);
		void destroy();
		void update();

		rml_ctx_type& get_ctx() { return ctx; }
	};
}
