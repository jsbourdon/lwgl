#include <pch.h>
#include "CommandProcessing.h"

void ProcessGfxCommands(CommandQueueHandle cmdQueue, GfxCommandBuffer *pCmdBuffers, uint32_t cmdBufferCount)
{
    ID3D11DeviceContext *pContext = reinterpret_cast<ID3D11DeviceContext*>(cmdQueue);
}