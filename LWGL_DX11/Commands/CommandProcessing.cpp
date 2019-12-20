#include <pch.h>
#include "CommandProcessing.h"

void ExecuteCommandBuffers(CommandQueueHandle cmdQueue, const GfxCommandBuffer *pCmdBuffers, uint32_t bufferCount)
{
    ID3D11DeviceContext *pContext = reinterpret_cast<ID3D11DeviceContext*>(cmdQueue);
}

void SignalFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value)
{
    // Nothing to do in this platform. Fully handled by drivers.
}

void WaitForFence(CommandQueueHandle cmdQueue, FenceHandle fence, uint64_t value)
{
    // Nothing to do in this platform. Fully handled by drivers.
}