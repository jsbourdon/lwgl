#pragma once

#include "LWGL_Common/Core/Defines.h"
#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Commands/GfxCommandPacket.h"

using namespace lwgl;
using namespace lwgl::command;

extern "C"
{
    EXPORT_SYMBOL
    void ProcessGfxCommands(CommandQueueHandle cmdQueue, GfxCommandPacketList *pCmdBuffers, uint32_t cmdBufferCount);
}