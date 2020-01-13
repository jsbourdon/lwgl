#pragma once

#include "Salvation_Common/Core/Defines.h"
#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Commands/GfxCommandBuffer.h"

using namespace lwgl;
using namespace lwgl::command;

extern "C"
{
    EXPORT_SYMBOL
    void ExecuteCommandBuffers(CommandQueueHandle cmdQueue, const GfxCommandBuffer *pCmdBuffers, uint32_t bufferCount);

    EXPORT_SYMBOL
    void SignalFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value);

    EXPORT_SYMBOL
    void WaitForFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value);
}