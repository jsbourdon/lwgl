// LWGL_TestApp.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "LWGL_TestApp.h"
#include <cstdint>
#include "LWGL/WindowFactory/WindowFactory.h"
#include "LWGL/Events/MessagePump.h"
#include "LWGL_Common/Resources/ResourceHandles.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    lwgl::WindowHandle hwnd = lwgl::factory::CreateNewWindow(reinterpret_cast<lwgl::AppHandle>(hInstance), 1920, 1080);
    lwgl::factory::DisplayWindow(hwnd);

    return lwgl::events::PumpMessages();
}
