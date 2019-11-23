// LWGL_TestApp.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "LWGL_TestApp.h"
#include <cstdint>
#include <thread>
#include <chrono>
#include "LWGL/WindowFactory/WindowFactory.h"
#include "LWGL/Events/MessagePump.h"
#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL/Device/GfxPlatform.h"
#include "LWGL/Device/GpuDevice.h"

using namespace lwgl;
using namespace lwgl::external;
using namespace std::chrono_literals;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowHandle hwnd = factory::CreateNewWindow(reinterpret_cast<lwgl::AppHandle>(hInstance), 1920, 1080);
    factory::DisplayWindow(hwnd);

    GpuDevice *pDevice = GpuDevice::CreateDevice(GfxPlatform::D3D11);

    while (!lwgl::events::PumpMessages())
    {
        std::this_thread::sleep_for(100ms);
    }

    GpuDevice::DestroyDevice(pDevice);

    return 0;
}
