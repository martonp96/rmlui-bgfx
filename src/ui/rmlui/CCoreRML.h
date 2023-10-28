#pragma once
#include "pch.h"

#include "CRenderInterface.h"
#include "CSystemInterface.h"
#include "CEventInstancer.h"
#include "CFileInterface.h"

namespace window
{
    class CWindow;
}

namespace ui
{
    class CCoreRML
    {
    private:
        typedef std::unique_ptr<Rml::Context, std::function<void(Rml::Context*)>> rml_ctx_type;
        rml_ctx_type m_ctx;
        
        std::unique_ptr<CRenderInterface> m_render_interface;
        std::unique_ptr<CSystemInterface> m_system_interface;
        std::unique_ptr<CEventInstancer> m_event_instancer;
        std::unique_ptr<CFileInterface> m_file_interface;

        Rml::ElementDocument* m_doc;

    public:
        CCoreRML(window::CWindow* window, const Eigen::Vector2i& size);
        ~CCoreRML();

        Rml::ElementDocument* CreateDocument(const std::string& path);
        Rml::ElementDocument* CreateDocumentFromMemory(const std::string& data);
        
        void Resize(const Eigen::Vector2i& size);
        void Render();

        Rml::Context* GetContext() const { return m_ctx.get(); }
        Rml::ElementDocument* GetDocument() const { return m_doc; }
    };
}
