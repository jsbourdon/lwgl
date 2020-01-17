#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Resources/ResourceTypes.h"
#include "LWGL_Common/Commands/GfxCommandBuffer.h"

using namespace lwgl::command;

namespace lwgl
{
    namespace external
    {
        namespace device
        {
            typedef GpuDeviceHandle(*CreateGfxDeviceFnctPtr)();
            typedef void (*DestroyGfxDeviceFnctPtr)(lwgl::GpuDeviceHandle);
            typedef CommandQueueHandle(*CreateCommandQueueFnctPtr)(lwgl::GpuDeviceHandle, lwgl::resources::CommandQueueType);
            typedef void (*DestroyCommandQueueFnctPtr)(lwgl::CommandQueueHandle);
            typedef CommandBufferHandle(*CreateCommandBufferFnctPtr)(lwgl::GpuDeviceHandle, lwgl::resources::CommandBufferType);
            typedef void (*DestroyCommandBufferFnctPtr)(lwgl::CommandBufferHandle);
        }

        namespace queue
        {
            typedef void (*ExecuteCommandBuffersFnctPtr)(CommandQueueHandle, const GfxCommandBuffer*, uint32_t);
            typedef void (*SignalFenceFnctPtr)(CommandQueueHandle, FenceHandle, uint64_t);
            typedef void (*WaitForFenceFnctPtr)(CommandQueueHandle, FenceHandle, uint64_t);
        }
    }
}