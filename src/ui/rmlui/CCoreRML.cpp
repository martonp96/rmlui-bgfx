#include "CCoreRML.h"

void ui::CCoreRML::create(SDL_Window* window, int width, int height)
{
	m_render_interface = std::make_unique<CRenderInterface>(width, height);
	m_system_interface = std::make_unique<CSystemInterface>(window);

	Rml::SetRenderInterface(m_render_interface.get());
	Rml::SetSystemInterface(m_system_interface.get());

	if (!Rml::Initialise())
		printf("failed to initialize rmlui\n");

	auto ctx_ptr = Rml::CreateContext("main", Rml::Vector2i(width, height));
	m_ctx = rml_ctx_type(ctx_ptr, [](Rml::Context* ctx) { Rml::RemoveContext("main"); });

	Rml::Debugger::Initialise(ctx_ptr);
	//Rml::Debugger::SetVisible(true);

	Rml::LoadFontFace("arial.ttf", true);
	auto doc = ctx_ptr->LoadDocument("main.rml");
	doc->Show();
}

void ui::CCoreRML::resize(int width, int height)
{
	m_ctx->SetDimensions(Rml::Vector2i(width, height));
	m_render_interface->Resize(width, height);
}

void ui::CCoreRML::destroy()
{
	Rml::Shutdown();
}

void ui::CCoreRML::update()
{
	if (!m_ctx)
		return;
	
	m_ctx->Update();
	m_ctx->Render();
}
