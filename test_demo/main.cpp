#include <cstdio>
#include <functional>
#include <string>

#include "api/api_base.h"

ElemBase* elem = nullptr;
AppBase* wnd = nullptr;
DocBase* doc = nullptr;

bool event_handler(ElemBase* element, std::string name, std::unordered_map<std::string, t_api_variant> parameters)
{
    printf("%p, %s, %d\n", element, name.c_str(), parameters.size());

    /*for(int i = 0; i < rml::dict::get_dict_size(parameters); i++)
    {
        printf("%s\n", rml::dict::get_key(parameters, i));
    }*/

    return true;
}

void RenderInitHandler()
{
    wnd->LoadFontFace("assets/LatoLatin-Regular.ttf", false);
    wnd->LoadFontFace("assets/LatoLatin-Italic.ttf", false);
    wnd->LoadFontFace("assets/LatoLatin-Bold.ttf", false);
    wnd->LoadFontFace("assets/LatoLatin-BoldItalic.ttf", false);
    wnd->LoadFontFace("assets/NotoEmoji-Regular.ttf", true);

    doc = wnd->LoadDocument("basic/demo/data/demo.rml");
    if(doc)
    {
        const auto title_elem = doc->GetElementById("title");
        title_elem->SetInnerRML("test title");

        if(const auto sandbox_src = doc->GetElementById("sandbox_rml_source"))
        {
            std::string value(sandbox_src->FormControlGetValue());
            value += "<p>Write your RML here</p>\n\n<!-- <img src=\"assets/high_scores_alien_1.tga\"/> -->";
            sandbox_src->FormControlSetValue(value);
        }

        if (const auto sandbox_tgt = doc->GetElementById("sandbox_target"))
        {
            const auto iframe_doc = wnd->CreateDocument("");
            const auto parent_node = iframe_doc->GetParent();
            const auto iframe_ptr = parent_node->RemoveChild(iframe_doc);

            sandbox_tgt->AppendChild(iframe_ptr);
            iframe_doc->SetProperty("position", "absolute");
            iframe_doc->SetProperty("display", "block");
            iframe_doc->SetInnerRML("<p>Rendered output goes here.</p>");
        }

        doc->SetVisible(true);
    }
}

int main()
{
    wnd = Core::CreateApp(1800, 890);

    wnd->RegisterRmlEventHandler(event_handler);
    wnd->RegisterRenderInitEventHandler(RenderInitHandler);

    wnd->Start();

    while (wnd->IsRunning())
        wnd->RunLoop();

    wnd->Destroy();
    delete wnd;

    return 0;
}