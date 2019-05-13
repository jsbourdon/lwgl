#include "GfxDeviceContext.h"
#include "../Resources/Mesh.h"
#include "../Pipeline/GfxPipeline.h"
#include "../Resources/InputLayout.h"
#include "../Resources/Shader.h"
#include "../Resources/BlendState.h"
#include "../Resources/DepthStencilState.h"
#include "../Resources/Buffer.h"
#include "../Resources/SamplerState.h"
#include "../Resources/RasterizerState.h"

using namespace lwgl;
using namespace core;

const D3D11_MAP GfxDeviceContext::s_MapTypes[] =
{
    D3D11_MAP_READ,
    D3D11_MAP_WRITE,
    D3D11_MAP_READ_WRITE,
    D3D11_MAP_WRITE_DISCARD,
    D3D11_MAP_WRITE_NO_OVERWRITE
};

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
    m_pD3DContext->RSSetState(pPipeline->m_pRasterizerState->m_pD3DRasterizerState);
    m_pD3DContext->OMSetBlendState(pPipeline->m_pBlendState->m_pBlendState, NULL, 0xFFFFFFFF);
    m_pD3DContext->OMSetDepthStencilState(pPipeline->m_pDepthStencilState->m_pDepthStencilState, pPipeline->m_StencilRef);
}

void GfxDeviceContext::DrawMesh(Mesh* mesh)
{
    mesh->m_DXUTMesh.Render(m_pD3DContext, 0);
}

void* GfxDeviceContext::MapBuffer(Buffer *pBuffer, MapType mapType)
{
    HRESULT hr;

    D3D11_MAP d3dMapType = s_MapTypes[size_t(mapType)];
    D3D11_MAPPED_SUBRESOURCE subRsc;
    CHECK_HRESULT_PTR(m_pD3DContext->Map(pBuffer->m_pD3DBuffer, 0, d3dMapType, 0, &subRsc));

    return subRsc.pData;
}

void GfxDeviceContext::UnmapBuffer(Buffer *pBuffer)
{
    m_pD3DContext->Unmap(pBuffer->m_pD3DBuffer, 0);
}

void GfxDeviceContext::BindBuffer(Buffer *pBuffer, Stage stage, uint32_t slot)
{
    switch (stage)
    {
    case Stage::VS:
        m_pD3DContext->VSSetConstantBuffers(slot, 1, &pBuffer->m_pD3DBuffer);
        break;
    case Stage::PS:
        m_pD3DContext->PSSetConstantBuffers(slot, 1, &pBuffer->m_pD3DBuffer);
        break;
    }
}

void GfxDeviceContext::BindSampler(SamplerState *pSampler, Stage stage, uint32_t slot)
{
    switch (stage)
    {
    case Stage::VS:
        m_pD3DContext->VSSetSamplers(slot, 1, &pSampler->m_pSamplerState);
        break;
    case Stage::PS:
        m_pD3DContext->PSSetSamplers(slot, 1, &pSampler->m_pSamplerState);
        break;
    }
}
