#pragma once

#include "LWGL_Common/ExternalFunctions/ExternalFunctions.h"

namespace lwgl
{
    namespace external
    {
        using LibraryHandle = lwgl::LibraryHandle;

        struct ExternalFunctions
        {
            //CreateWindowFnctPtr                 m_CreateWindowFnctPtr { nullptr };
            //ProcessWindowSystemEventsFnctPtr    m_ProcessWSEventsFnctPtr { nullptr };
            //
            //WindowHandle    CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height);
            //void            ProcessWindowSystemEvents();
        };

        enum class GfxPlatform
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

            static ExternalFunctions LoadExternalFunctions(GfxPlatform gfx);

        private:

            static void LoadGfxFunctions(ExternalFunctions &functions);

            // Platform-dependent implementations required.
            // Look into `Library/Implementations/<platform>.inl`
            static LibraryHandle LoadDynamicLibrary(const wchar_t *name);
            static void* GetFunctionAddress(LibraryHandle lib, const char *functionName);

            static ExternalFunctions    g_Functions;
            static LibraryHandle        g_LoadedGfxLibrary;
            static const wchar_t*       g_GfxPlatformLibNames[];

            LibraryLoader() = delete;
            ~LibraryLoader() = delete;
        };
    }
}