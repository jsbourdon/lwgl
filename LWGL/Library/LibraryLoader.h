#pragma once

#include "LWGL/Device/GfxPlatform.h"
#include "LWGL_Common/ExternalFunctions/ExternalFunctions.h"
#include "LWGL_Common/Resources/ResourceTypes.h"

using namespace lwgl::external::device;
using namespace lwgl::external::queue;
using namespace lwgl::resources;

namespace lwgl
{
    namespace external
    {
        using LibraryHandle = lwgl::LibraryHandle;

        struct GpuDeviceFunctions
        {
            GpuDeviceHandle     CreateGfxDevice();
            void                DestroyGfxDevice(GpuDeviceHandle hdl);
            CommandQueueHandle  CreateCommandQueue(GpuDeviceHandle hdl, CommandQueueType type);
            void                DestroyCommandQueue(CommandQueueHandle hdl);
            CommandBufferHandle CreateCommandBuffer(GpuDeviceHandle hdl, CommandBufferType type);
            void                DestroyCommandBuffer(CommandBufferHandle hdl);

            LibraryHandle               m_LibHandle { 0 };
            CreateGfxDeviceFnctPtr      m_CreateGfxDeviceFnctPtr { nullptr };
            DestroyGfxDeviceFnctPtr     m_DestroyGfxDeviceFnctPtr { nullptr };
            CreateCommandQueueFnctPtr   m_CreateCmdQueueFnctPtr { nullptr };
            DestroyCommandQueueFnctPtr  m_DestroyCmdQueueFnctPtr { nullptr };
            CreateCommandBufferFnctPtr  m_CreateCommandBufferFnctPtr { nullptr };
            DestroyCommandBufferFnctPtr m_DestroyCommandBufferFnctPtr { nullptr };
        };

        struct CommandQueueFunctions
        {
            void ExecuteCommandBuffers(CommandQueueHandle cmdQueue, const GfxCommandBuffer *pBuffers, uint32_t count);
            void SignalFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value);
            void WaitForFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value);

            LibraryHandle                   m_LibHandle { 0 };
            ExecuteCommandBuffersFnctPtr    m_ExecuteCmdBuffersFnctPtr { nullptr };
            SignalFenceFnctPtr              m_SignalFenceFnctPtr { nullptr };
            WaitForFenceFnctPtr             m_WaitForFenceFnctPtr { nullptr };
        };

        class LibraryLoader
        {
        public:

            static void Init(GfxPlatform gfx);
            static void Shutdown();
            static GpuDeviceFunctions LoadGpuDeviceFunctions();
            static CommandQueueFunctions LoadCommandQueueFunctions();

        private:

            template<typename T>
            inline static T LoadFunction(const char *pFnctName);

            // Platform-dependent implementations required.
            // Look into `Library/Implementations/<platform>.inl`
            static LibraryHandle LoadDynamicLibrary(const wchar_t *name);
            static void UnloadDynamicLibrary(LibraryHandle handle);
            static void* GetFunctionAddress(LibraryHandle lib, const char *functionName);

            static const wchar_t* s_GfxPlatformLibNames[];
            static LibraryHandle s_PlatformLibHdl;

            LibraryLoader() = delete;
            ~LibraryLoader() = delete;
        };
    }
}