#pragma once

#include <stdint.h>
#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace command
    {
        struct GfxCommandPacket
        {
            RenderPassHandle        m_RenderPass;
            FrameBufferHandle       m_FrameBuffer;
            GfxPipelineHandle       m_Pipeline;
            IndexBufferHandle       m_IndexBuffer;
            uintptr_t               m_VertexBufferCount;
            uintptr_t               m_DescriptorTableCount;
            uintptr_t*              m_ResourceHandles;
        };

        struct GfxCommandPacketList
        {
            GfxCommandPacket *m_pPackets;
            uint32_t m_Count;
        };
    }
}