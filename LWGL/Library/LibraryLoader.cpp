#include <pch.h>
#include <type_traits>
#include "LibraryLoader.h"

using namespace lwgl;
using namespace lwgl::external;

ExternalFunctions   LibraryLoader::g_Functions {};
LibraryHandle       LibraryLoader::g_LoadedLibrary { NULL };

const wchar_t* LibraryLoader::g_PlatformLibNames[] =
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

WindowHandle ExternalFunctions::CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height)
{
    return m_CreateWindowFnctPtr(owner, width, height);
}

ExternalFunctions LibraryLoader::LoadExternalFunctions(Platform platform)
{
    static_assert(std::extent_v<decltype(LibraryLoader::g_PlatformLibNames)> == size_t(Platform::Count));

    if (!g_LoadedLibrary)
    {
        LoadDynamicLibrary(g_PlatformLibNames[size_t(platform)]);
        GetExternalFunctions();
    }

    return g_Functions;
}

void LibraryLoader::GetExternalFunctions()
{
    LoadCreateNewWindowFnct(g_Functions);
}

void LibraryLoader::LoadCreateNewWindowFnct(ExternalFunctions &functions)
{
    functions.m_CreateWindowFnctPtr = reinterpret_cast<CreateWindowFnctPtr>(GetFunctionAddress(g_LoadedLibrary, "CreateNewWindow"));
}

#include INCLUDE_IMPLEMENTATION(LibraryLoader)
