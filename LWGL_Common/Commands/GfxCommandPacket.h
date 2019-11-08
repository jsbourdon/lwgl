#pragma once

#include "../Resources/ResourceHandles.h"

namespace lwgl
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
}