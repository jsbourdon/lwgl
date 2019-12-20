#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Commands/GfxCommandPacket.h"

namespace lwgl
{
    namespace command
    {
        struct GfxCommandBuffer
        {
            CommandBufferHandle m_CmdbufferHdl { Handle_NULL };
            void*               m_pPackets { nullptr };
        };
    }
}