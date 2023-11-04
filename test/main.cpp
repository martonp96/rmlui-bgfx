#include <cstdio>
#include <functional>
#include <string>

#include "api/api_base.h"

ElemBase* elem = nullptr;
AppBase* wnd = nullptr;

bool event_handler(ElemBase* element, std::string name, std::unordered_map<std::string, t_api_variant> parameters)
{
    printf("%p, %s, %d\n", element, name.c_str(), parameters.size());

    if(name == "click" && elem)
    {
        auto size = parameters.size();
        auto variant_x = parameters["mouse_x"];
        auto variant_y = parameters["mouse_y"];

        auto mouse_x = std::get<int64_t>(variant_x);
        auto mouse_y = std::get<int64_t>(variant_y);

        printf("%f, %f\n", mouse_x, mouse_y);

        elem->SetProperty("left", (std::to_string(mouse_x) + "px").c_str());
        elem->SetProperty("top", (std::to_string(mouse_y) + "px").c_str());
        elem->SetProperty("position", "absolute");
    }

    /*for(int i = 0; i < rml::dict::get_dict_size(parameters); i++)
    {
        printf("%s\n", rml::dict::get_key(parameters, i));
    }*/

    return true;
}

void RenderInitHandler()
{
    wnd->LoadFontFace("arial.ttf", true);

    auto doc = wnd->LoadDocument("main.rml");
    doc->SetVisible(true);
    
    elem = doc->CreateTextNode("test");
    doc->AppendChild(elem);
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