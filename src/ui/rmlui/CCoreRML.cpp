#include "CCoreRML.h"
#include "window/CWindow.h"

ui::CCoreRML::CCoreRML(window::CWindow* window, const Eigen::Vector2i& size)
{
    m_render_interface = std::make_unique<CRenderInterface>(size);
    m_system_interface = std::make_unique<CSystemInterface>(window->GetWindowPtr());
    m_event_instancer = std::make_unique<CEventInstancer>(window);

    Rml::SetRenderInterface(m_render_interface.get());
    Rml::SetSystemInterface(m_system_interface.get());

    Rml::Factory::RegisterEventInstancer(m_event_instancer.get());

    if (!Rml::Initialise())
        SPDLOG_ERROR("Failed to initialize RmlUI");

    auto ctx_ptr = Rml::CreateContext("main", Rml::Vector2i(size[0], size[1]));
    m_ctx = rml_ctx_type(ctx_ptr, [](Rml::Context* ctx) { Rml::RemoveContext("main"); });

    Rml::Debugger::Initialise(ctx_ptr);
    //Rml::Debugger::SetVisible(true);

    Rml::LoadFontFace("arial.ttf", true);
    //auto doc = m_ctx->LoadDocument("main.rml");
    //doc->Show();
}

ui::CCoreRML::~CCoreRML()
{
    Rml::Shutdown();
}

Rml::ElementDocument* ui::CCoreRML::CreateDocument(const std::string& path)
{
    m_doc = m_ctx->LoadDocument(path);
    return m_doc;
}

Rml::ElementDocument* ui::CCoreRML::CreateDocumentFromMemory(const std::string& data)
{
    m_doc = m_ctx->LoadDocumentFromMemory(data);
    return m_doc;
}

void ui::CCoreRML::Resize(const Eigen::Vector2i& size)
{
    m_ctx->SetDimensions(Rml::Vector2i(size[0], size[1]));
    m_render_interface->Resize(size);
}
void ui::CCoreRML::Render()
{
    if (!m_ctx)
        return;

    m_ctx->Update();
    m_ctx->Render();
}