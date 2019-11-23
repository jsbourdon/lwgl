#include <pch.h>
#include <type_traits>
#include "LibraryLoader.h"

using namespace lwgl;
using namespace lwgl::external;

GpuDeviceHandle ExternalFunctions::CreateGfxDevice()
{
    return (*m_CreateGfxDeviceFnctPtr)();
}

void ExternalFunctions::DestroyGfxDevice(GpuDeviceHandle hdl)
{
    (*m_DestroyGfxDeviceFnctPtr)(hdl);
}

CommandQueueHandle ExternalFunctions::CreateCommandQueue(CommandQueueType type)
{
    return (*m_CreateCmdQueueFnctPtr)(type);
}

void ExternalFunctions::DestroyCommandQueue(CommandQueueHandle hdl)
{
    (*m_DestroyCmdQueueFnctPtr)(hdl);
}

const wchar_t* LibraryLoader::g_GfxPlatformLibNames[] =
{
    L"LWGL_DX11",
    L"LWGL_DX12",
    L"LWGL_Vulkan",
    L"LWGL_MoltenVK",
    L"LWGL_NX",
    L"LWGL_Neo",
    L"LWGL_Scorpio",
    L"LWGL_Prospero",
    L"LWGL_Scarlett",
};

ExternalFunctions LibraryLoader::LoadExternalFunctions(GfxPlatform gfx)
{
    static_assert(std::extent_v<decltype(LibraryLoader::g_GfxPlatformLibNames)> == size_t(GfxPlatform::Count));

    ExternalFunctions functions;
    functions.m_LibHandle = LoadDynamicLibrary(g_GfxPlatformLibNames[size_t(gfx)]);
    LoadGfxFunctions(functions);

    return functions;
}

void LibraryLoader::UnloadExternalFunctions(ExternalFunctions &functions)
{
    UnloadDynamicLibrary(functions.m_LibHandle);
    memset(&functions, 0, sizeof(ExternalFunctions));
}

void LibraryLoader::LoadGfxFunctions(ExternalFunctions &functions)
{
    functions.m_CreateGfxDeviceFnctPtr = reinterpret_cast<CreateGfxDeviceFnctPtr>(GetFunctionAddress(functions.m_LibHandle, "CreateGfxDevice"));
    functions.m_DestroyGfxDeviceFnctPtr = reinterpret_cast<DestroyGfxDeviceFnctPtr>(GetFunctionAddress(functions.m_LibHandle, "DestroyGfxDevice"));
    functions.m_CreateCmdQueueFnctPtr = reinterpret_cast<CreateCommandQueueFnctPtr>(GetFunctionAddress(functions.m_LibHandle, "CreateCommandQueue"));
    functions.m_DestroyCmdQueueFnctPtr = reinterpret_cast<DestroyCommandQueueFnctPtr>(GetFunctionAddress(functions.m_LibHandle, "DestroyCommandQueue"));
}

#include INCLUDE_IMPLEMENTATION(LibraryLoader)
