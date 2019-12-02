#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Commands/GfxCommandPacket.h"

namespace lwgl
{
    namespace command
    {
        struct GfxCommandBuffer
        {
            GfxCommandBufferHandle m_CmdbufferHdl { Handle_NULL };
            uint8_t *m_pPackets { nullptr };
            uint32_t m_PacketCount { 0 };
        };
    }
}