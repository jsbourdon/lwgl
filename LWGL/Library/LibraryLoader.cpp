#include <pch.h>
#include <type_traits>
#include "LibraryLoader.h"

using namespace lwgl;
using namespace lwgl::external;

ExternalFunctions   LibraryLoader::g_Functions {};
LibraryHandle       LibraryLoader::g_LoadedGfxLibrary { NULL };

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

    if (!g_LoadedGfxLibrary)
    {
        g_LoadedGfxLibrary = LoadDynamicLibrary(g_GfxPlatformLibNames[size_t(gfx)]);
        LoadGfxFunctions(g_Functions);
    }

    return g_Functions;
}

void LibraryLoader::LoadGfxFunctions(ExternalFunctions &functions)
{
    //functions.m_CreateWindowFnctPtr = reinterpret_cast<CreateWindowFnctPtr>(GetFunctionAddress(g_LoadedGfxLibrary, "CreateNewWindow"));
    //functions.m_ProcessWSEventsFnctPtr = reinterpret_cast<ProcessWindowSystemEventsFnctPtr>(GetFunctionAddress(g_LoadedOSLibrary, "CreateNewWindow"));
}

#include INCLUDE_IMPLEMENTATION(LibraryLoader)
