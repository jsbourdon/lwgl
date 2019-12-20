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
#include "LWGL_Common/SystemInfo/SystemInfo.h"
#include "LWGL_Common/Memory/VirtualMemoryAllocator.h"
#include "LWGL_Common/Memory/ThreadHeapAllocator.h"
#include "LWGL_Common/Threading/WorkerThread.h"
#include "LWGL/Device/GfxPlatform.h"
#include "LWGL/Device/GpuDevice.h"

using namespace lwgl;
using namespace lwgl::external;
using namespace lwgl::memory;
using namespace lwgl::threading;
using namespace std::chrono_literals;

static int WorkerFunction(int iterationCount)
{
    int total = 0;
    for (int i = 0; i < iterationCount; ++i)
    {
        total += i;
    }

    return total;
}

typedef int (WorkerFnctType)(int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ThreadHeapAllocator::Init(GiB(1), MiB(100));

    WorkerThread<WorkerFnctType> worker0(WorkerFunction, GiB(1), MiB(100));
    WorkerThread<WorkerFnctType> worker1(WorkerFunction, GiB(1), MiB(100));
    worker0.Init();
    worker1.Init();

    worker0.Run(1000);
    worker1.Run(100);

    int value0 = worker0.Wait();
    int value1 = worker1.Wait();

    WindowHandle hwnd = factory::CreateNewWindow(reinterpret_cast<lwgl::AppHandle>(hInstance), 1920, 1080);
    factory::DisplayWindow(hwnd);

    LibraryLoader::Init(GfxPlatform::D3D11);
    GpuDevice *pDevice = GpuDevice::CreateDevice();
    
    while (!lwgl::events::PumpMessages())
    {
        std::this_thread::sleep_for(100ms);
    }

    GpuDevice::DestroyDevice(pDevice);

    return 0;
}
