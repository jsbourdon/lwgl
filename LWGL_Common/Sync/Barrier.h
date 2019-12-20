#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace sync
    {
        enum class ResourceStates
        {
            Unknown = 0x000000,
            Common = 0x000001,
            VertexBuffer = 0x000002,
            ConstantBuffer = 0x000004,
            IndexBuffer = 0x000008,
            RenderTarget = 0x000010,
            UAV = 0x000020,
            DepthWrite = 0x000040,
            DepthRead = 0x000080,
            NonPixelShaderRsc = 0x000100,
            PixelShaderRsc = 0x000200,
            StreamOut = 0x000400,
            IndirectArg = 0x000800,
            CopyDest = 0x001000,
            CopySrc = 0x002000,
            ResolveDest = 0x004000,
            ResolveSrc = 0x008000,
        };

        template<typename HandleType>
        struct TransitionBarrier
        {
            HandleType      m_ResourceHdl { Handle_NULL };
            ResourceStates  m_BeforeState { ResourceStates::Unknown };
            ResourceStates  m_AfterState { ResourceStates::Unknown };
            uint32_t        m_SubRscIndex { 0 };
        };

        template<typename HandleType>
        struct AliasingBarrier
        {
            HandleType  m_ResourceBeforeHdl { Handle_NULL };
            HandleType  m_ResourceAfterHdl { Handle_NULL };
        };

        using TextureTransitionBarrier  = TransitionBarrier<TextureHandle>;
        using BufferTransitionBarrier   = TransitionBarrier<BufferHandle>;

        using TextureAliasingBarrier    = AliasingBarrier<TextureHandle>;
        using BufferAliasingBarrier     = AliasingBarrier<BufferHandle>;

        struct BarrierSet
        {
            TextureTransitionBarrier*   m_pTextureTransitions;
            BufferTransitionBarrier*    m_pBufferTransitions;
            TextureAliasingBarrier*     m_pTextureAliasing;
            BufferAliasingBarrier*      m_pBufferAliasing;
            Handle*                     m_pTextureUAV;
            Handle*                     m_pBufferUAV;
        };
    }
}