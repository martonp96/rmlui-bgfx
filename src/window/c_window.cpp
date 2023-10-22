#include "c_window.h"

using namespace window;

void c_window::create(int x, int y, int width, int height)
{
    m_wnd_pos = { x, y };
    m_wnd_size = { width, height };

    WNDCLASSEXA wc = { sizeof(wc), CS_CLASSDC, wnd_proc, 0L, sizeof(c_window), GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "app_class", nullptr};
    if(!RegisterClassExA(&wc))
        printf("error registering window class\n");

    h_wnd = CreateWindowA(wc.lpszClassName, "app_window", WS_OVERLAPPEDWINDOW, m_wnd_pos[0], m_wnd_pos[1], m_wnd_size[0], m_wnd_size[1], nullptr, nullptr, wc.hInstance, nullptr);
    if (!h_wnd)
        printf("error creating the window\n");

    SetWindowLongPtrA(h_wnd, 0, (LONG_PTR)this);

    //FreeConsole();
}

void c_window::destroy()
{
    if(h_wnd)
		DestroyWindow(h_wnd);

    UnregisterClassA("app_class", GetModuleHandle(nullptr));
}

void c_window::start()
{
    ShowWindow(h_wnd, SW_SHOWDEFAULT);
    UpdateWindow(h_wnd);

    MSG msg{ };
    while (msg.message != WM_QUIT) 
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            continue;
        }

        if(update_cb)
            update_cb();
    }
}

LRESULT c_window::wnd_proc(HWND h_wnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    if (u_msg == WM_CLOSE || u_msg == WM_DESTROY) PostQuitMessage(0);

    const auto window = (c_window*)GetWindowLongPtrA(h_wnd, 0);
    if (!window)
        return DefWindowProcA(h_wnd, u_msg, w_param, l_param);

	if(u_msg == WM_SIZE)
	{
        if (w_param == SIZE_MINIMIZED)
        {
			//TODO: minimize_cb
        }
        else
        {
            window->m_wnd_size[0] = LOWORD(l_param);
        	window->m_wnd_size[1] = HIWORD(l_param);
            if (window->wnd_resize_cb)
                window->wnd_resize_cb(window->m_wnd_size);
        }
	}

    if (window->wnd_proc_cb)
        window->wnd_proc_cb(h_wnd, u_msg, w_param, l_param);

    return DefWindowProcA(h_wnd, u_msg, w_param, l_param);
}
