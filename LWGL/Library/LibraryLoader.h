#pragma once

#include "LWGL_Common/ExternalFunctions/ExternalFunctions.h"

namespace lwgl
{
    namespace external
    {
        using LibraryHandle = lwgl::LibraryHandle;

        struct ExternalFunctions
        {
            CreateWindowFnctPtr m_CreateWindowFnctPtr { nullptr };

            WindowHandle CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height);
        };

        enum class Platform
        {
            Unknown = -1,
            D3D11,
            D3D12,
            Vulkan,
            MoltenVK,
            NX,
            Neo,
            Scorpio,
            Prospero,
            Scarlett,
            Count
        };

        class LibraryLoader
        {
        public:

            static ExternalFunctions LoadExternalFunctions(Platform platform);

        private:

            static void GetExternalFunctions();
            static void LoadCreateNewWindowFnct(ExternalFunctions &functions);

            // Platform-dependent implementations required.
            // Look into `Library/Implementations/<platform>.inl`
            static void LoadDynamicLibrary(const wchar_t *name);
            static void* GetFunctionAddress(LibraryHandle lib, const char *functionName);

            static ExternalFunctions    g_Functions;
            static LibraryHandle        g_LoadedLibrary;
            static const wchar_t*       g_PlatformLibNames[];

            LibraryLoader() = delete;
            ~LibraryLoader() = delete;
        };
    }
}