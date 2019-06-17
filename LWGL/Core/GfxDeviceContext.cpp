#include <pch.h>

#include "GfxDeviceContext.h"
#include "../Descriptors/ClearDescriptor.h"
#include "../Descriptors/BufferDescriptor.h"
#include "../Resources/Mesh.h"
#include "../Pipeline/GfxPipeline.h"
#include "../Resources/InputLayout.h"
#include "../Resources/Shader.h"
#include "../Resources/BlendState.h"
#include "../Resources/DepthStencilState.h"
#include "../Resources/Buffer.h"
#include "../Resources/SamplerState.h"
#include "../Resources/RasterizerState.h"
#include "../Resources/Texture.h"

using namespace lwgl;
using namespace core;
using namespace descriptors;

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
    , m_pCurrentPipeline(nullptr)
    , m_pRenderTargets {}
    , m_RenderTargetCount(0)
{

}

GfxDeviceContext::~GfxDeviceContext()
{
    SAFE_RELEASE(m_pCurrentPipeline);

    for (uint32_t rtIndex = 0; rtIndex < m_RenderTargetCount; ++rtIndex)
    {
        SAFE_RELEASE(m_pRenderTargets[rtIndex]);
    }
}

GfxNativeDeviceContext* GfxDeviceContext::GetNativeContext()
{
    return m_pD3DContext;
}

void GfxDeviceContext::SetupPipeline(GfxPipeline *pPipeline)
{
    if (m_pCurrentPipeline)
    {
        m_pCurrentPipeline->Release();
    }

    pPipeline->AddRef();
    m_pCurrentPipeline = pPipeline;

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
    D3D11_MAP d3dMapType = s_MapTypes[size_t(mapType)];
    D3D11_MAPPED_SUBRESOURCE subRsc;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DContext->Map(pBuffer->m_pD3DBuffer, 0, d3dMapType, 0, &subRsc), nullptr);

    return subRsc.pData;
}

void GfxDeviceContext::UnmapBuffer(Buffer *pBuffer)
{
    m_pD3DContext->Unmap(pBuffer->m_pD3DBuffer, 0);
}

void GfxDeviceContext::BindBuffer(const Buffer *pBuffer, Stage stage, uint32_t slot)
{
    switch (stage)
    {
    case Stage::VS:
        BindBufferToVSStage(pBuffer, slot);
        break;
    case Stage::PS:
        BindBufferToPSStage(pBuffer, slot);
        break;
    }
}

void GfxDeviceContext::BindTexture(const Texture *pTexture, Stage stage, uint32_t slot)
{
    switch (stage)
    {
    case Stage::VS:
        m_pD3DContext->VSSetShaderResources(slot, 1, &pTexture->m_pSRV);
        break;
    case Stage::PS:
        m_pD3DContext->PSSetShaderResources(slot, 1, &pTexture->m_pSRV);
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

void GfxDeviceContext::BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount)
{
    assert(renderTargetCount <= lwgl::core::MAX_RENDERTARGET_COUNT);

    UnbindRenderTargets();

    ID3D11RenderTargetView **pRTVs = static_cast<ID3D11RenderTargetView**>(StackAlloc(sizeof(ID3D11RenderTargetView*)));
    for (uint32_t i = 0; i < renderTargetCount; ++i)
    {
        Texture *pRenderTarget = pRenderTargets[i];
        pRenderTarget->AddRef();
        m_pRenderTargets[i] = pRenderTarget;

        pRTVs[i] = pRenderTarget->m_pRTV;
    }

    m_RenderTargetCount = renderTargetCount;
    m_pD3DContext->OMSetRenderTargets(renderTargetCount, pRTVs, DXUTGetD3D11DepthStencilView());
}

void GfxDeviceContext::UnbindRenderTargets()
{
    for (uint32_t rtIndex = 0; rtIndex < m_RenderTargetCount; ++rtIndex)
    {
        SAFE_RELEASE(m_pRenderTargets[rtIndex]);
    }

    m_RenderTargetCount = 0;
}

void GfxDeviceContext::BindSwapChain()
{
    UnbindRenderTargets();
    DXUTSetupD3D11Views(m_pD3DContext);
}

void GfxDeviceContext::Clear(const ClearDescriptor &desc)
{
    if (desc.ClearColor)
    {
        if (m_RenderTargetCount > 0)
        {
            for (uint32_t rtIndex = 0; rtIndex < m_RenderTargetCount; ++rtIndex)
            {
                Texture *pRenderTarget = m_pRenderTargets[rtIndex];
                ID3D11RenderTargetView *pRTV = pRenderTarget->m_pRTV;
                m_pD3DContext->ClearRenderTargetView(pRTV, desc.ColorClearValue);
            }
        }
        else
        {
            ID3D11RenderTargetView *pRTV = DXUTGetD3D11RenderTargetView();
            m_pD3DContext->ClearRenderTargetView(pRTV, desc.ColorClearValue);
        }
    }

    if (desc.ClearDepth || desc.ClearStencil)
    {
        ID3D11DepthStencilView *pDSV = DXUTGetD3D11DepthStencilView();
        D3D11_CLEAR_FLAG clearFlags = static_cast<D3D11_CLEAR_FLAG>((desc.ClearDepth ? D3D11_CLEAR_DEPTH : 0) | (desc.ClearStencil ? D3D11_CLEAR_STENCIL : 0));
        m_pD3DContext->ClearDepthStencilView(pDSV, clearFlags, desc.DepthClearValue, desc.StencilClearValue);
    }
}

void GfxDeviceContext::BindBufferToVSStage(const Buffer *pBuffer, uint32_t slot)
{
    switch (pBuffer->m_BufferType)
    {
    case BufferType::Constants:
        m_pD3DContext->VSSetConstantBuffers(slot, 1, &pBuffer->m_pD3DBuffer);
        break;
    case BufferType::Structured:
        m_pD3DContext->VSSetShaderResources(slot, 1, &pBuffer->m_pD3DBufferSRV);
        break;
    }
}

void GfxDeviceContext::BindBufferToPSStage(const Buffer *pBuffer, uint32_t slot)
{
    switch (pBuffer->m_BufferType)
    {
    case BufferType::Constants:
        m_pD3DContext->PSSetConstantBuffers(slot, 1, &pBuffer->m_pD3DBuffer);
        break;
    case BufferType::Structured:
        m_pD3DContext->PSSetShaderResources(slot, 1, &pBuffer->m_pD3DBufferSRV);
        break;
    }
}
