#include <pch.h>

#include "Pipeline/GfxPipeline.h"
#include "Resources/InputLayout.h"
#include "Resources/Shader.h"
#include "Resources/BlendState.h"
#include "Resources/DepthStencilState.h"
#include "Resources/RasterizerState.h"

using namespace lwgl;
using namespace pipeline;

GfxPipeline::GfxPipeline()
    : base()
    , m_pInputLayout(nullptr)
    , m_pVertexShader(nullptr)
    , m_pFragmentShader(nullptr)
    , m_pBlendState(nullptr)
    , m_pDepthStencilState(nullptr)
    , m_pRasterizerState(nullptr)
    , m_StencilRef(0)
{
}

GfxPipeline::~GfxPipeline()
{
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pFragmentShader);
    SAFE_RELEASE(m_pBlendState);
    SAFE_RELEASE(m_pDepthStencilState);
    SAFE_RELEASE(m_pRasterizerState);
}

void GfxPipeline::SetStencilRef(uint32_t stencilRef)
{
    m_StencilRef = stencilRef;
}
