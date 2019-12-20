#pragma once

#include <stdint.h>
#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace command
    {
        enum class CommandPacketType
        {
            IA_Setup,
            DS_Setup,
            RT_Setup,
            BlendFactor_Setup,
            Viewport_Setup,
            Scissor_Setup,
            Pipe_Setup,
            DescHeap_Setup,
            DescTable_Setup,
            Barriers,
            Clear,
            DrawIndexed,
            EOS
        };

        struct IASetup
        {
            IndexBufferHandle   m_IndexBuffer;
            uint32_t            m_VertexBufferCount;
        };

        struct DepthStencilSetup
        {
            TextureHandle   m_DepthStencil;
            uint32_t        m_StencilRef;
        };

        struct RenderTargetSetup
        {
            uint32_t m_RenderTargetCount;
        };

        struct BlendFactorSetup
        {
            float m_BlendFactors[4];
        };

        struct ViewportSetup
        {
            uint32_t m_ViewportCount;
        };

        struct ScissorSetup
        {
            uint32_t m_ScissorRectCount;
        };

        struct PipeSetup
        {
            GfxPipelineHandle   m_Pipeline;
            RenderPassHandle    m_RenderPass;
        };

        struct DescHeapSetup
        {
            uint32_t m_DescriptorHeapCount;
        };

        struct DescTableSetup
        {
            uint32_t m_DescriptorTableCount;
        };

        struct BarrierSetup
        {
            uint32_t m_TextureUAVBarrierCount;
            uint32_t m_TextureTransitionBarrierCount;
            uint32_t m_TextureAliasingBarrierCount;
            uint32_t m_BufferUAVBarrierCount;
            uint32_t m_BufferTransitionBarrierCount;
            uint32_t m_BufferAliasingBarrierCount;
        };

        struct ClearOp
        {
            TextureHandle m_Target;
        };

        struct DrawIndexedOp
        {
            uint32_t m_IndexCount;
            uint32_t m_StartIndex;
            uint32_t m_BaseVertexOffset;
        };

        struct GfxCommandPacket
        {
            CommandPacketType       m_PacketType;

            union
            {
                IASetup             m_IASetup;
                DepthStencilSetup   m_DSSetup;
                RenderTargetSetup   m_RTSetup;
                BlendFactorSetup    m_BlendFactorSetup;
                ViewportSetup       m_ViewportSetup;
                ScissorSetup        m_ScissorSetup;
                PipeSetup           m_PipeSetup;
                DescHeapSetup       m_DescHeapSetup;
                DescTableSetup      m_DescTableSetup;
                BarrierSetup        m_BarrierSetup;
                ClearOp             m_ClearOp;
                DrawIndexedOp       m_DrawIndexedOp;
            };

            uintptr_t               m_SetupAdditionalData;
        };
    }
}