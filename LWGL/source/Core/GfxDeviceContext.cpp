#include <pch.h>

#include "Core/GfxDeviceContext.h"
#include "Descriptors/ClearDescriptor.h"
#include "Descriptors/BufferDescriptor.h"
#include "Resources/Mesh.h"
#include "Pipeline/GfxPipeline.h"
#include "Resources/InputLayout.h"
#include "Resources/Shader.h"
#include "Resources/BlendState.h"
#include "Resources/DepthStencilState.h"
#include "Resources/Buffer.h"
#include "Resources/SamplerState.h"
#include "Resources/RasterizerState.h"
#include "Resources/Texture.h"
#include "Resources/TextureArray.h"

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

void* GfxDeviceContext::s_pNullResources[lwgl::core::MAX_SHADERRESOURCE_COUNT] {};

GfxDeviceContext::GfxDeviceContext(ID3D11DeviceContext* d3dContext)
    : m_pD3DContext(d3dContext)
{
    static_assert(ARRAYSIZE(s_pNullResources) == lwgl::core::MAX_SHADERRESOURCE_COUNT, "Invalid number of NULL resources");
    static_assert(lwgl::core::MAX_RENDERTARGET_COUNT <= lwgl::core::MAX_SHADERRESOURCE_COUNT, "MAX_RENDERTARGET_COUNT is greater than MAX_SHADERRESOURCE_COUNT");
}

GfxDeviceContext::~GfxDeviceContext()
{
    SAFE_RELEASE(m_pCurrentPipeline);
    SAFE_RELEASE(m_pSwapChainDepthStencil);
    SAFE_RELEASE(m_pCurrentDepthStencil);
    SAFE_RELEASE(m_pFullScreenTriangleVS);
    SAFE_RELEASE(m_pFullScreenTiangleInputLayout);

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
    mesh->m_DXUTMesh.Render(m_pD3DContext, mesh->m_AlbedoSlot);
}

void GfxDeviceContext::DrawFullScreenTriangle()
{
    assert(m_pFullScreenTriangleVS != nullptr);
    assert(m_pFullScreenTiangleInputLayout != nullptr);

    m_pD3DContext->IASetInputLayout(m_pFullScreenTiangleInputLayout->m_pLayout);
    m_pD3DContext->VSSetShader(m_pFullScreenTriangleVS->m_pVertexShader, nullptr, 0);
    m_pD3DContext->Draw(3, 0);
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

void GfxDeviceContext::SetDepthStencil(Texture *pDepthStencil, uint32_t arrayIndex)
{
    SAFE_ADDREF(pDepthStencil);
    SAFE_RELEASE(m_pCurrentDepthStencil);
    m_pCurrentDepthStencil = pDepthStencil;
    m_DepthStencilArrayIndex = arrayIndex;
}

void GfxDeviceContext::BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, bool bindDepthStencil)
{
    BindRenderTargets(pRenderTargets, renderTargetCount, bindDepthStencil ? m_pSwapChainDepthStencil : nullptr);
}

void GfxDeviceContext::BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, Texture *pDepthBuffer)
{
    assert(renderTargetCount <= lwgl::core::MAX_RENDERTARGET_COUNT);

    ID3D11DepthStencilView *pDSV = pDepthBuffer != nullptr ? pDepthBuffer->m_pDSV : nullptr;

    UnbindRenderTargets();
    SetDepthStencil(pDepthBuffer);
    SetViewport((renderTargetCount > 0) ? pRenderTargets[0] : pDepthBuffer);
    BindRenderTargets(pRenderTargets, renderTargetCount, pDSV);
}

void GfxDeviceContext::BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, ID3D11DepthStencilView *pDepthBufferView)
{
    assert(renderTargetCount <= lwgl::core::MAX_RENDERTARGET_COUNT);

    m_RenderTargetCount = renderTargetCount;

    ID3D11RenderTargetView **pRTVs = (renderTargetCount > 0) ? static_cast<ID3D11RenderTargetView * *>(StackAlloc(sizeof(ID3D11RenderTargetView*) * renderTargetCount)) : nullptr;

    for (uint32_t i = 0; i < renderTargetCount; ++i)
    {
        Texture *pRenderTarget = pRenderTargets[i];
        pRenderTarget->AddRef();
        m_pRenderTargets[i] = pRenderTarget;

        pRTVs[i] = pRenderTarget->m_pRTV;
    }

    m_pD3DContext->OMSetRenderTargets(renderTargetCount, pRTVs, pDepthBufferView);
}

void GfxDeviceContext::BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, TextureArray *pDepthBuffer, uint32_t depthArrayIndex)
{
    assert(depthArrayIndex < pDepthBuffer->m_ArraySize);

    ID3D11DepthStencilView *pDSV = pDepthBuffer->m_pDSVs[depthArrayIndex];

    UnbindRenderTargets();
    SetDepthStencil(pDepthBuffer, depthArrayIndex);
    SetViewport((renderTargetCount > 0) ? pRenderTargets[0] : pDepthBuffer);
    BindRenderTargets(pRenderTargets, renderTargetCount, pDSV);
}

void GfxDeviceContext::BindRenderTargets(TextureArray *pRenderTargets, uint32_t rtStartIndex, uint32_t renderTargetCount, TextureArray *pDepthBuffer, uint32_t depthArrayIndex)
{
    assert(pRenderTargets != nullptr);
    assert(pDepthBuffer != nullptr);
    assert(depthArrayIndex < pDepthBuffer->m_ArraySize);
    assert((rtStartIndex + renderTargetCount) <= uint32_t(pRenderTargets->m_ArraySize));

    UnbindRenderTargets();
    SetDepthStencil(pDepthBuffer, depthArrayIndex);
    SetViewport(pRenderTargets);

    m_pD3DContext->OMSetRenderTargets(renderTargetCount, pRenderTargets->m_pRTVs + rtStartIndex, pDepthBuffer->m_pDSVs[depthArrayIndex]);;
}

void GfxDeviceContext::BindSwapChainDepthStencilToStage(Stage stage, uint32_t slot)
{
    if (stage == Stage::VS)
    {
        m_pD3DContext->VSSetShaderResources(slot, 1, &m_pSwapChainDepthStencil->m_pSRV);
    }
    else if (stage == Stage::PS)
    {
        m_pD3DContext->PSSetShaderResources(slot, 1, &m_pSwapChainDepthStencil->m_pSRV);
    }
}

void GfxDeviceContext::UnbindRenderTargets()
{
    for (uint32_t rtIndex = 0; rtIndex < m_RenderTargetCount; ++rtIndex)
    {
        SAFE_RELEASE(m_pRenderTargets[rtIndex]);
    }

    m_pD3DContext->OMSetRenderTargets(m_RenderTargetCount, reinterpret_cast<ID3D11RenderTargetView**>(s_pNullResources), nullptr);

    m_RenderTargetCount = 0;
}

void GfxDeviceContext::SetViewport(float width, float height)
{
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pD3DContext->RSSetViewports(1, &vp);
}

void GfxDeviceContext::SetViewport(Texture *pTexture)
{
    if (pTexture != nullptr)
    {
        SetViewport(float(pTexture->m_Width), float(pTexture->m_Height));
    }
}

void GfxDeviceContext::BindSwapChain(bool bindDepthStencil)
{
    UnbindRenderTargets();

    const DXGI_SURFACE_DESC *pSurfaceDesc = DXUTGetDXGIBackBufferSurfaceDesc();
    SetViewport(float(pSurfaceDesc->Width), float(pSurfaceDesc->Height));
    SetDepthStencil(bindDepthStencil ? m_pSwapChainDepthStencil : nullptr);

    ID3D11RenderTargetView *pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView *pDSV = bindDepthStencil ? m_pSwapChainDepthStencil->m_pDSV : nullptr;
    m_pD3DContext->OMSetRenderTargets(1, &pRTV, pDSV);
}

void GfxDeviceContext::Unbind(Stage stage, uint32_t slot)
{
    if (stage == Stage::VS)
    {
        m_pD3DContext->VSSetShaderResources(slot, 1, nullptr);
    }
    else if (stage == Stage::PS)
    {
        m_pD3DContext->PSSetShaderResources(slot, 1, nullptr);
    }
}

void GfxDeviceContext::UnbindRange(Stage stage, uint32_t slot, uint32_t count)
{
    if (stage == Stage::VS)
    {
        m_pD3DContext->VSSetShaderResources(slot, count, reinterpret_cast<ID3D11ShaderResourceView**>(s_pNullResources));
    }
    else if (stage == Stage::PS)
    {
        m_pD3DContext->PSSetShaderResources(slot, count, reinterpret_cast<ID3D11ShaderResourceView * *>(s_pNullResources));
    }
}

void GfxDeviceContext::SetSwapChainDepthStencil(Texture *pDepthStencil)
{
    SAFE_RELEASE(m_pSwapChainDepthStencil);
    m_pSwapChainDepthStencil = pDepthStencil;
}

void GfxDeviceContext::SetFullScreenTriangleResources(Shader *pShader, InputLayout *pLayout)
{
    SAFE_RELEASE(m_pFullScreenTriangleVS);
    SAFE_RELEASE(m_pFullScreenTiangleInputLayout);

    m_pFullScreenTriangleVS = pShader;
    m_pFullScreenTiangleInputLayout = pLayout;
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

    if ((desc.ClearDepth || desc.ClearStencil) && m_pCurrentDepthStencil != nullptr)
    {
        assert((m_DepthStencilArrayIndex == 0) || m_DepthStencilArrayIndex < static_cast<TextureArray*>(m_pCurrentDepthStencil)->m_ArraySize);

        ID3D11DepthStencilView *pDSV = (m_DepthStencilArrayIndex > 0) ? static_cast<TextureArray*>(m_pCurrentDepthStencil)->m_pDSVs[m_DepthStencilArrayIndex] : m_pCurrentDepthStencil->m_pDSV;
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
