#pragma once

#include "Core/RefCountedObject.h"
#include "Descriptors/PipelineDescriptor.h"

namespace lwgl
{
    namespace resources
    {
        class InputLayout;
        class Shader;
        class BlendState;
        class DepthStencilState;
        class RasterizerState;
    }

    using namespace descriptors;

    namespace pipeline
    {
        class GfxPipeline : public RefCountedObject<GfxPipeline>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        public:

            void SetStencilRef(uint32_t stencilRef);

        private:

            GfxPipeline();
            ~GfxPipeline();

            InputLayout*        m_pInputLayout;
            Shader*             m_pVertexShader;
            Shader*             m_pFragmentShader;
            BlendState*         m_pBlendState;
            DepthStencilState*  m_pDepthStencilState;
            RasterizerState*    m_pRasterizerState;
            uint32_t            m_StencilRef;
        };
    }
}