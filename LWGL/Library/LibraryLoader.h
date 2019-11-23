#pragma once

#include "LWGL/Device/GfxPlatform.h"
#include "LWGL_Common/ExternalFunctions/ExternalFunctions.h"

namespace lwgl
{
    namespace external
    {
        using namespace gfx;
        using namespace resources;

        using LibraryHandle = lwgl::LibraryHandle;

        struct ExternalFunctions
        {
            LibraryHandle m_LibHandle { 0 };
            CreateGfxDeviceFnctPtr m_CreateGfxDeviceFnctPtr { nullptr };
            DestroyGfxDeviceFnctPtr m_DestroyGfxDeviceFnctPtr { nullptr };
            CreateCommandQueueFnctPtr m_CreateCmdQueueFnctPtr { nullptr };
            DestroyCommandQueueFnctPtr m_DestroyCmdQueueFnctPtr { nullptr };

            GpuDeviceHandle CreateGfxDevice();
            void DestroyGfxDevice(GpuDeviceHandle hdl);
            CommandQueueHandle CreateCommandQueue(CommandQueueType type);
            void DestroyCommandQueue(CommandQueueHandle hdl);
        };

        class LibraryLoader
        {
        public:

            static ExternalFunctions LoadExternalFunctions(GfxPlatform gfx);
            static void UnloadExternalFunctions(ExternalFunctions &functions);

        private:

            static void LoadGfxFunctions(ExternalFunctions &functions);

            // Platform-dependent implementations required.
            // Look into `Library/Implementations/<platform>.inl`
            static LibraryHandle LoadDynamicLibrary(const wchar_t *name);
            static void UnloadDynamicLibrary(LibraryHandle handle);
            static void* GetFunctionAddress(LibraryHandle lib, const char *functionName);

            static const wchar_t* g_GfxPlatformLibNames[];

            LibraryLoader() = delete;
            ~LibraryLoader() = delete;
        };
    }
}