#include "GfxDeviceContext.h"
#include "../Resources/Mesh.h"
#include "../Pipeline/GfxPipeline.h"
#include "../Resources/InputLayout.h"
#include "../Resources/Shader.h"
#include "../Resources/BlendState.h"
#include "../Resources/DepthStencilState.h"

using namespace lwgl;
using namespace core;

GfxDeviceContext::GfxDeviceContext(ID3D11DeviceContext* d3dContext)
    : m_pD3DContext(d3dContext)
{

}

GfxDeviceContext::~GfxDeviceContext()
{

}

void GfxDeviceContext::SetupPipeline(const GfxPipeline *pPipeline)
{
    m_pD3DContext->IASetInputLayout(pPipeline->m_pInputLayout->m_pLayout);
    m_pD3DContext->VSSetShader(pPipeline->m_pVertexShader->m_pVertexShader, nullptr, 0);
    m_pD3DContext->PSSetShader(pPipeline->m_pFragmentShader->m_pFragmentShader, nullptr, 0);
    m_pD3DContext->OMSetBlendState(pPipeline->m_pBlendState->m_pBlendState, NULL, 0xFFFFFFFF);
    m_pD3DContext->OMSetDepthStencilState(pPipeline->m_pDepthStencilState->m_pDepthStencilState, pPipeline->m_StencilRef);
}

void GfxDeviceContext::DrawMesh(Mesh* mesh)
{
    mesh->m_DXUTMesh.Render(m_pD3DContext);
}
