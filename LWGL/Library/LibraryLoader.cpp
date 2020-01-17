#include <pch.h>
#include <type_traits>
#include "LibraryLoader.h"

using namespace lwgl;
using namespace lwgl::external;

///////////////////////////////////////////////////////////////////////////
// GpuDeviceFunctions
///////////////////////////////////////////////////////////////////////////
GpuDeviceHandle GpuDeviceFunctions::CreateGfxDevice()
{
    return (*m_CreateGfxDeviceFnctPtr)();
}

void GpuDeviceFunctions::DestroyGfxDevice(GpuDeviceHandle hdl)
{
    (*m_DestroyGfxDeviceFnctPtr)(hdl);
}

CommandQueueHandle GpuDeviceFunctions::CreateCommandQueue(lwgl::GpuDeviceHandle hdl, CommandQueueType type)
{
    return (*m_CreateCmdQueueFnctPtr)(hdl, type);
}

void GpuDeviceFunctions::DestroyCommandQueue(CommandQueueHandle hdl)
{
    (*m_DestroyCmdQueueFnctPtr)(hdl);
}

CommandBufferHandle GpuDeviceFunctions::CreateCommandBuffer(GpuDeviceHandle hdl, CommandBufferType type)
{
    return (*m_CreateCommandBufferFnctPtr)(hdl, type);
}

void GpuDeviceFunctions::DestroyCommandBuffer(CommandBufferHandle hdl)
{
    (*m_DestroyCommandBufferFnctPtr)(hdl);
}

///////////////////////////////////////////////////////////////////////////
// CommandQueueFunctions
///////////////////////////////////////////////////////////////////////////
void CommandQueueFunctions::ExecuteCommandBuffers(CommandQueueHandle cmdQueue, const GfxCommandBuffer *pBuffers, uint32_t count)
{
    (*m_ExecuteCmdBuffersFnctPtr)(cmdQueue, pBuffers, count);
}

void CommandQueueFunctions::SignalFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value)
{
    (*m_SignalFenceFnctPtr)(cmdQueue, fence, value);
}

void CommandQueueFunctions::WaitForFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value)
{
    (*m_WaitForFenceFnctPtr)(cmdQueue, fence, value);
}

///////////////////////////////////////////////////////////////////////////
// LibraryLoader
///////////////////////////////////////////////////////////////////////////
const wchar_t* LibraryLoader::s_GfxPlatformLibNames[] =
{
    L"LWGL_DX11",
    L"LWGL_DX12",
    L"LWGL_Vulkan",
    L"LWGL_MoltenVK",
    L"LWGL_NX",
    L"LWGL_Neo",
    L"LWGL_Scorpio",
    L"LWGL_Prospero",
    L"LWGL_Lockhart",
    L"LWGL_Anaconda"
};

LibraryHandle LibraryLoader::s_PlatformLibHdl = Handle_NULL;

void LibraryLoader::Init(GfxPlatform gfx)
{
    static_assert(std::extent_v<decltype(LibraryLoader::s_GfxPlatformLibNames)> == size_t(GfxPlatform::Count));

    s_PlatformLibHdl = LoadDynamicLibrary(s_GfxPlatformLibNames[size_t(gfx)]);
}

void LibraryLoader::Shutdown()
{
    UnloadDynamicLibrary(s_PlatformLibHdl);
}

template<typename T>
void LibraryLoader::LoadFunction(const char *pFnctName, T &fnctPtr)
{
    fnctPtr = reinterpret_cast<T>(GetFunctionAddress(s_PlatformLibHdl, pFnctName));
}

GpuDeviceFunctions LibraryLoader::LoadGpuDeviceFunctions()
{
    GpuDeviceFunctions functions;
    functions.m_LibHandle = s_PlatformLibHdl;
    
    LoadFunction("CreateGfxDevice", functions.m_CreateGfxDeviceFnctPtr);
    LoadFunction("DestroyGfxDevice", functions.m_DestroyGfxDeviceFnctPtr);
    LoadFunction("CreateCommandQueue", functions.m_CreateCmdQueueFnctPtr);
    LoadFunction("DestroyCommandQueue", functions.m_DestroyCmdQueueFnctPtr);
    LoadFunction("CreateCommandBuffer", functions.m_CreateCommandBufferFnctPtr);
    LoadFunction("DestroyCommandBuffer", functions.m_DestroyCommandBufferFnctPtr);

    return functions;
}

CommandQueueFunctions LibraryLoader::LoadCommandQueueFunctions()
{
    CommandQueueFunctions functions;
    functions.m_LibHandle = s_PlatformLibHdl;

    LoadFunction("ExecuteCommandBuffers", functions.m_ExecuteCmdBuffersFnctPtr);
    LoadFunction("SignalFence", functions.m_SignalFenceFnctPtr);
    LoadFunction("WaitForFence", functions.m_WaitForFenceFnctPtr);

    return functions;
}

#include INCLUDE_IMPLEMENTATION(LibraryLoader)
