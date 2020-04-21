#include <pch.h>

#include "Core/GfxDevice.h"

#include "Descriptors/ShaderDescriptor.h"
#include "Descriptors/BlendStateDescriptor.h"
#include "Descriptors/InputLayoutDescriptor.h"
#include "Descriptors/PixelFormats.h"
#include "Descriptors/PipelineDescriptor.h"
#include "Descriptors/BufferDescriptor.h"
#include "Descriptors/TextureDescriptor.h"

#include "Resources/Mesh.h"
#include "Resources/Buffer.h"
#include "Resources/Shader.h"
#include "Resources/BlendState.h"
#include "Resources/InputLayout.h"
#include "Resources/DepthStencilState.h"
#include "Resources/Texture.h"
#include "Resources/TextureArray.h"
#include "Resources/SamplerState.h"
#include "Resources/RasterizerState.h"

#include "Pipeline/GfxPipeline.h"

using namespace lwgl;
using namespace core;
using namespace descriptors;

const char *const GfxDevice::s_ShaderModels[] =
{
    "vs_5_0",
    "ps_5_0"
};

const DXGI_FORMAT GfxDevice::s_PixelFormats[] =
{
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32B32A32_FLOAT,
    DXGI_FORMAT_R32G32B32A32_UINT,
    DXGI_FORMAT_R32G32B32A32_SINT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32_UINT,
    DXGI_FORMAT_R32G32B32_SINT,
    DXGI_FORMAT_R16G16B16A16_FLOAT,
    DXGI_FORMAT_R16G16B16A16_UNORM,
    DXGI_FORMAT_R16G16B16A16_UINT,
    DXGI_FORMAT_R16G16B16A16_SNORM,
    DXGI_FORMAT_R16G16B16A16_SINT,
    DXGI_FORMAT_R32G32_FLOAT,
    DXGI_FORMAT_R32G32_UINT,
    DXGI_FORMAT_R32G32_SINT,
    DXGI_FORMAT_R10G10B10A2_UNORM,
    DXGI_FORMAT_R10G10B10A2_UINT,
    DXGI_FORMAT_R11G11B10_FLOAT,
    DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
    DXGI_FORMAT_R8G8B8A8_UINT,
    DXGI_FORMAT_R8G8B8A8_SNORM,
    DXGI_FORMAT_R8G8B8A8_SINT,
    DXGI_FORMAT_R16G16_FLOAT,
    DXGI_FORMAT_R16G16_UNORM,
    DXGI_FORMAT_R16G16_UINT,
    DXGI_FORMAT_R16G16_SNORM,
    DXGI_FORMAT_R16G16_SINT,
    DXGI_FORMAT_D32_FLOAT,
    DXGI_FORMAT_R32_FLOAT,
    DXGI_FORMAT_R32_UINT,
    DXGI_FORMAT_R32_SINT,
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
    DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_R8G8_UINT,
    DXGI_FORMAT_R8G8_SNORM,
    DXGI_FORMAT_R8G8_SINT,
    DXGI_FORMAT_R16_FLOAT,
    DXGI_FORMAT_D16_UNORM,
    DXGI_FORMAT_R16_UNORM,
    DXGI_FORMAT_R16_UINT,
    DXGI_FORMAT_R16_SNORM,
    DXGI_FORMAT_R16_SINT,
    DXGI_FORMAT_R8_UNORM,
    DXGI_FORMAT_R8_UINT,
    DXGI_FORMAT_R8_SNORM,
    DXGI_FORMAT_R8_SINT,
    DXGI_FORMAT_A8_UNORM,
    DXGI_FORMAT_R1_UNORM,
    DXGI_FORMAT_BC1_UNORM,
    DXGI_FORMAT_BC1_UNORM_SRGB,
    DXGI_FORMAT_BC2_UNORM,
    DXGI_FORMAT_BC2_UNORM_SRGB,
    DXGI_FORMAT_BC3_UNORM,
    DXGI_FORMAT_BC3_UNORM_SRGB,
    DXGI_FORMAT_BC4_UNORM,
    DXGI_FORMAT_BC4_SNORM,
    DXGI_FORMAT_BC5_UNORM,
    DXGI_FORMAT_BC5_SNORM,
    DXGI_FORMAT_B5G6R5_UNORM,
    DXGI_FORMAT_B5G5R5A1_UNORM,
    DXGI_FORMAT_B8G8R8A8_UNORM,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
};

const D3D11_BLEND GfxDevice::s_BlendValues[] =
{
    D3D11_BLEND_ZERO,
    D3D11_BLEND_ONE,
    D3D11_BLEND_SRC_COLOR,
    D3D11_BLEND_INV_SRC_COLOR,
    D3D11_BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA,
    D3D11_BLEND_DEST_ALPHA,
    D3D11_BLEND_INV_DEST_ALPHA,
    D3D11_BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR,
};

const D3D11_BLEND_OP GfxDevice::s_BlendOperations[] =
{
    D3D11_BLEND_OP_ADD,
    D3D11_BLEND_OP_SUBTRACT,
    D3D11_BLEND_OP_REV_SUBTRACT,
    D3D11_BLEND_OP_MIN,
    D3D11_BLEND_OP_MAX
};

const LPCSTR GfxDevice::s_SemanticNames[] =
{
    "POSITION",
    "NORMAL",
    "TANGENT",
    "TEXCOORD",
    "TEXCOORD",
    "TEXCOORD",
    "TEXCOORD",
};

const DXGI_FORMAT GfxDevice::s_InputLayoutFormats[] =
{
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32_FLOAT,
    DXGI_FORMAT_R32G32_FLOAT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32A32_FLOAT,
};

const D3D11_STENCIL_OP GfxDevice::s_StencilOps[]
{
    D3D11_STENCIL_OP_KEEP,
    D3D11_STENCIL_OP_ZERO,
    D3D11_STENCIL_OP_REPLACE,
    D3D11_STENCIL_OP_INCR_SAT,
    D3D11_STENCIL_OP_DECR_SAT,
    D3D11_STENCIL_OP_INVERT,
    D3D11_STENCIL_OP_INCR,
    D3D11_STENCIL_OP_DECR
};

const D3D11_COMPARISON_FUNC GfxDevice::s_ComparisonFuncs[] =
{
    D3D11_COMPARISON_NEVER,
    D3D11_COMPARISON_LESS,
    D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_LESS_EQUAL,
    D3D11_COMPARISON_GREATER,
    D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL,
    D3D11_COMPARISON_ALWAYS
};

const uint32_t GfxDevice::s_BufferBindFlags[] =
{
    D3D11_BIND_CONSTANT_BUFFER,
    D3D11_BIND_SHADER_RESOURCE,
    D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
    D3D11_BIND_VERTEX_BUFFER,
    D3D11_BIND_INDEX_BUFFER
};

const uint32_t GfxDevice::s_CPUAccessFlags[] =
{
    0,
    0,
    D3D11_CPU_ACCESS_WRITE,
    D3D11_CPU_ACCESS_READ
};

const D3D11_USAGE GfxDevice::s_ResourceUsages[] =
{
    D3D11_USAGE_DEFAULT,
    D3D11_USAGE_IMMUTABLE,
    D3D11_USAGE_DYNAMIC,
    D3D11_USAGE_STAGING,
};

const D3D11_CULL_MODE GfxDevice::s_CullModes[] =
{
    D3D11_CULL_NONE,
    D3D11_CULL_FRONT,
    D3D11_CULL_BACK
};

GfxDevice::GfxDevice(ID3D11Device* d3dDevice)
    : m_pD3DDevice(d3dDevice)
{
    static_assert(ARRAYSIZE(s_PixelFormats) == size_t(PixelFormat::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_ShaderModels) == size_t(ShaderType::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_BlendValues) == size_t(BlendValue::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_BlendOperations) == size_t(BlendOperation::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_SemanticNames) == size_t(InputLayoutSemantic::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_InputLayoutFormats) == size_t(InputLayoutSemantic::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_StencilOps) == size_t(StencilOperation::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_ComparisonFuncs) == size_t(ComparisonFunction::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_BufferBindFlags) == size_t(BufferType::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_CPUAccessFlags) == size_t(ResourceUsage::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_ResourceUsages) == size_t(ResourceUsage::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_CullModes) == size_t(CullMode::EnumCount), "Array is missing values");
}

GfxDevice::~GfxDevice()
{
    
}

GfxNativeDevice* GfxDevice::GetNativeDevice()
{
    return m_pD3DDevice;
}

GfxPipeline* GfxDevice::CreatePipeline(const PipelineDescriptor &desc)
{
    GfxPipeline *pPipeline = new GfxPipeline();

    pPipeline->m_pVertexShader = CreateShader(desc.VertexShader);
    pPipeline->m_pFragmentShader = CreateShader(desc.FragmentShader);
    pPipeline->m_pInputLayout = CreateInputLayout(desc.InputLayout, pPipeline->m_pVertexShader);
    pPipeline->m_pBlendState = CreateBlendState(desc.BlendState);
    pPipeline->m_pDepthStencilState = CreateDepthStencilState(desc.DepthStencilState);
    pPipeline->m_pRasterizerState = CreateRasterizerState(desc.RasterizerState);

    return pPipeline;
}

Mesh* GfxDevice::CreateMesh(const wchar_t *filePath)
{
    Mesh* mesh = new Mesh();
    CDXUTSDKMesh &dxutMesh = mesh->m_DXUTMesh;
    dxutMesh.Create(m_pD3DDevice, filePath);

    SDKMESH_MATERIAL *pMat = dxutMesh.GetMaterial(0);
    mesh->m_AlbedoSlot = strlen(pMat->DiffuseTexture) > 0 ? 0 : -1;

    return mesh;
}

Buffer* GfxDevice::CreateBuffer(const BufferDescriptor &desc)
{
    D3D11_BUFFER_DESC d3dDesc = {};
    d3dDesc.ByteWidth = uint32_t(desc.ByteSize);
    d3dDesc.StructureByteStride = uint32_t(desc.StructureStride);
    d3dDesc.Usage = s_ResourceUsages[size_t(desc.Usage)];
    d3dDesc.BindFlags = s_BufferBindFlags[size_t(desc.Type)];
    d3dDesc.CPUAccessFlags = s_CPUAccessFlags[size_t(desc.Usage)];
    d3dDesc.MiscFlags = (desc.Type == BufferType::Structured) ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;

    ID3D11Buffer *pD3DBuffer = nullptr;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateBuffer(&d3dDesc, nullptr, &pD3DBuffer), nullptr);
    Buffer *pBuffer = new Buffer();
    pBuffer->m_pD3DBuffer = pD3DBuffer;
    pBuffer->m_BufferType = desc.Type;

    if (desc.DebugName)
    {
        DXUT_SetDebugName(pD3DBuffer, desc.DebugName);
    }

    // Create SRV if necessary
    if (desc.Type == BufferType::Structured)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = uint32_t(desc.ByteSize / desc.StructureStride);

        ID3D11ShaderResourceView *pSrv = nullptr;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateShaderResourceView(pD3DBuffer , &srvDesc, &pSrv), pBuffer);
        pBuffer->m_pD3DBufferSRV = pSrv;
    }

    return pBuffer;
}

Texture* GfxDevice::CreateTextureViews(Texture *pTexture, const TextureDescriptor &desc)
{
    ID3D11Texture2D *pD3DTexture = pTexture->m_pTexture;
    const uint32_t descFlags = uint32_t(desc.BindFlags);

    const bool needsSRV = !!(descFlags & uint32_t(TextureBindFlags::ShaderResource));
    const bool needsRTV = !!(descFlags & uint32_t(TextureBindFlags::RenderTarget));
    const bool needsUAV = !!(descFlags & uint32_t(TextureBindFlags::UnorderedAccessView));
    const bool needsDSV = (desc.Type == TextureType::DepthStencil);

    const NativePixelFormat pixelFormat = ConvertToNativePixelFormat(desc.Format);

    if (needsSRV)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Format = pixelFormat;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;

        ID3D11ShaderResourceView *pSrv;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateShaderResourceView(pD3DTexture, &srvDesc, &pSrv), pTexture);
        pTexture->m_pSRV = pSrv;
    }

    if (needsDSV)
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDSV.Flags = 0;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;

        ID3D11DepthStencilView *pDsv;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateDepthStencilView(pD3DTexture, &descDSV, &pDsv), pTexture);
        pTexture->m_pDSV = pDsv;
    }
    else if (needsRTV)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = pixelFormat;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        ID3D11RenderTargetView *pRtv;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateRenderTargetView(pD3DTexture, &rtvDesc, &pRtv), pTexture);
        pTexture->m_pRTV = pRtv;
    }

    if (needsUAV)
    {
        // #todo Implement Texture UAV creation
    }

    return pTexture;
}

TextureArray* GfxDevice::CreateTextureViews(TextureArray *pTexture, const TextureDescriptor &desc)
{
    assert(desc.ArraySize > 1);

    ID3D11Texture2D *pD3DTexture = pTexture->m_pTexture;
    const uint32_t arraySize = desc.ArraySize;
    const uint32_t descFlags = uint32_t(desc.BindFlags);

    const bool needsSRV = !!(descFlags & uint32_t(TextureBindFlags::ShaderResource));
    const bool needsRTV = !!(descFlags & uint32_t(TextureBindFlags::RenderTarget));
    const bool needsUAV = !!(descFlags & uint32_t(TextureBindFlags::UnorderedAccessView));
    const bool needsDSV = (desc.Type == TextureType::DepthStencil);

    const NativePixelFormat pixelFormat = ConvertToNativePixelFormat(desc.Format);

    pTexture->m_ArraySize = arraySize;

    if (needsSRV)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
        srvDesc.Format = pixelFormat;
        srvDesc.Texture2DArray.ArraySize = arraySize;
        srvDesc.Texture2DArray.FirstArraySlice = 0;
        srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
        srvDesc.Texture2DArray.MostDetailedMip = 0;

        ID3D11ShaderResourceView *pSrv;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateShaderResourceView(pD3DTexture, &srvDesc, &pSrv), pTexture);
        pTexture->m_pSRV = pSrv;
    }

    if (needsDSV)
    {
        pTexture->m_pDSVs = new ID3D11DepthStencilView*[arraySize];

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDSV.Flags = 0;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
        descDSV.Texture2DArray.ArraySize = 1;
        descDSV.Texture2DArray.MipSlice = 0;

        for (uint32_t i = 0; i < arraySize; ++i)
        {
            descDSV.Texture2DArray.FirstArraySlice = i;

            ID3D11DepthStencilView *pDsv;
            CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateDepthStencilView(pD3DTexture, &descDSV, &pDsv), pTexture);
            pTexture->m_pDSVs[i] = pDsv;
        }

        pTexture->m_pDSV = pTexture->m_pDSVs[0];
    }
    else if (needsRTV)
    {
        pTexture->m_pRTVs = new ID3D11RenderTargetView*[arraySize];

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = pixelFormat;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        rtvDesc.Texture2DArray.ArraySize = 1;
        rtvDesc.Texture2DArray.MipSlice = 0;

        for (uint32_t i = 0; i < arraySize; ++i)
        {
            rtvDesc.Texture2DArray.FirstArraySlice = i;

            ID3D11RenderTargetView *pRtv;
            CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateRenderTargetView(pD3DTexture, &rtvDesc, &pRtv), pTexture);
            pTexture->m_pRTVs[i] = pRtv;
        }

        pTexture->m_pRTV = pTexture->m_pRTVs[0];
    }

    if (needsUAV)
    {
        // #todo Implement Texture UAV creation
    }

    return pTexture;
}

template<typename T> T* GfxDevice::CreateTextureCore(const TextureDescriptor &desc)
{
    assert(desc.Type == TextureType::Texture2D || desc.Type == TextureType::DepthStencil);
    assert(desc.Usage != ResourceUsage::GPU_ReadOnly);  // Immutable textures not supported yet

    uint32_t bindFlags = 0;
    uint32_t descFlags = uint32_t(desc.BindFlags);

    const bool needsSRV = !!(descFlags & uint32_t(TextureBindFlags::ShaderResource));
    const bool needsRTV = !!(descFlags & uint32_t(TextureBindFlags::RenderTarget));
    const bool needsUAV = !!(descFlags & uint32_t(TextureBindFlags::UnorderedAccessView));
    const bool needsDSV = (desc.Type == TextureType::DepthStencil);

    bindFlags |= needsSRV ? D3D11_BIND_SHADER_RESOURCE : 0;
    bindFlags |= needsRTV ? D3D11_BIND_RENDER_TARGET : 0;
    bindFlags |= needsUAV ? D3D11_BIND_UNORDERED_ACCESS : 0;
    bindFlags |= needsDSV ? D3D11_BIND_DEPTH_STENCIL : 0;

    const NativePixelFormat pixelFormat = needsDSV ? DXGI_FORMAT_R24G8_TYPELESS : ConvertToNativePixelFormat(desc.Format);

    D3D11_TEXTURE2D_DESC d3dDesc;
    d3dDesc.Width = desc.Width;
    d3dDesc.Height = desc.Height;
    d3dDesc.MipLevels = desc.MipLevels;
    d3dDesc.ArraySize = desc.ArraySize;
    d3dDesc.Format = pixelFormat;
    d3dDesc.SampleDesc.Count = desc.SampleCount;
    d3dDesc.SampleDesc.Quality = 0;
    d3dDesc.Usage = s_ResourceUsages[size_t(desc.Usage)];
    d3dDesc.BindFlags = bindFlags;
    d3dDesc.CPUAccessFlags = s_CPUAccessFlags[size_t(desc.Usage)];
    d3dDesc.MiscFlags = 0;

    ID3D11Texture2D *pD3DTexture;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateTexture2D(&d3dDesc, nullptr, &pD3DTexture), nullptr);

    T *pTexture = new T();
    pTexture->m_pTexture = pD3DTexture;
    pTexture->m_Width = desc.Width;
    pTexture->m_Height = desc.Height;

    const char *debugName = desc.DebugName;
    if (debugName != nullptr)
    {
        DXUT_SetDebugName(pD3DTexture, debugName);
    }

    return pTexture;
}

Texture* GfxDevice::CreateTexture(const TextureDescriptor &desc)
{
    Texture *pTexture = CreateTextureCore<Texture>(desc);
    return CreateTextureViews(pTexture, desc);
}

TextureArray* GfxDevice::CreateTextureArray(const TextureDescriptor &desc)
{
    TextureArray *pTexture = CreateTextureCore<TextureArray>(desc);
    return CreateTextureViews(pTexture, desc);
}

SamplerState* GfxDevice::CreateSamplerState(const SamplerStateDescriptor &desc)
{
    D3D11_SAMPLER_DESC d3dDesc = {};
    d3dDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    d3dDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    d3dDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    d3dDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    ID3D11SamplerState *pD3DSamplerState = nullptr;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateSamplerState(&d3dDesc, &pD3DSamplerState), nullptr);
    SamplerState *pSamplerState = new SamplerState();
    pSamplerState->m_pSamplerState = pD3DSamplerState;

    return pSamplerState;
}

Shader* GfxDevice::CreateShader(const ShaderDescriptor &desc)
{
    assert(desc.Type == ShaderType::VertexShader || desc.Type == ShaderType::FragmentShader);

    Shader *pShader = new Shader();

    ShaderType type = desc.Type;
    const wchar_t *filePath = desc.FilePath;
    const char *code = desc.Code;
    const char *entryPoint = desc.EntryPoint;
    const char *debugName = desc.DebugName;

    pShader->m_Type = type;

    // NULL shader
    if (filePath == nullptr && code == nullptr)
    {
        return pShader;
    }

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    DWORD debugFlag = debugName != nullptr ? D3DCOMPILE_SKIP_OPTIMIZATION : 0;
    dwShaderFlags |= debugFlag;

    ID3DBlob* pShaderBuffer = nullptr;
    if (code != nullptr)
    {
        ID3DBlob* pErrorBuffer = nullptr;
        CHECK_HRESULT_RETURN_VALUE(D3DCompile(code, strlen(code), nullptr, nullptr, nullptr, entryPoint, 
            s_ShaderModels[size_t(type)], dwShaderFlags, 0, &pShaderBuffer, &pErrorBuffer), pShader);
    }
    else
    {
        CHECK_HRESULT_RETURN_VALUE(DXUTCompileFromFile(filePath, nullptr, entryPoint, s_ShaderModels[size_t(type)], dwShaderFlags, 0, &pShaderBuffer), pShader);
    }

    pShader->m_pShaderBuffer = pShaderBuffer;

    if (type == ShaderType::VertexShader)
    {
        ID3D11VertexShader *pVertexShader;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, &pVertexShader), pShader);

        pShader->m_pVertexShader = pVertexShader;

        if (debugName != nullptr)
        {
            DXUT_SetDebugName(pVertexShader, debugName);
        }
    }
    else if (type == ShaderType::FragmentShader)
    {
        ID3D11PixelShader *pFragmentShader;
        CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, &pFragmentShader), pShader);

        pShader->m_pFragmentShader = pFragmentShader;

        if (debugName != nullptr)
        {
            DXUT_SetDebugName(pFragmentShader, debugName);
        }
    }

    return pShader;
}

BlendState* GfxDevice::CreateBlendState(const BlendStateDescriptor &desc)
{
    BlendState *pBlendState = nullptr;

    D3D11_BLEND_DESC d3dDesc = {};
    d3dDesc.AlphaToCoverageEnable = false;
    d3dDesc.IndependentBlendEnable = true;
    
    uint32_t rtCount = desc.RenderTargetCount;
    for (uint32_t rtIndex = 0; rtIndex < rtCount; ++rtIndex)
    {
        D3D11_RENDER_TARGET_BLEND_DESC &rtBlendDesc = d3dDesc.RenderTarget[rtIndex];
        if (rtBlendDesc.BlendEnable = desc.IsEnabled)
        {
            rtBlendDesc.SrcBlend = s_BlendValues[size_t(desc.SourceColor)];
            rtBlendDesc.DestBlend = s_BlendValues[size_t(desc.DestinationColor)];
            rtBlendDesc.BlendOp = s_BlendOperations[size_t(desc.ColorOperation)];
            rtBlendDesc.SrcBlendAlpha = s_BlendValues[size_t(desc.SourceAlpha)];
            rtBlendDesc.DestBlendAlpha = s_BlendValues[size_t(desc.DestinationAlpha)];
            rtBlendDesc.BlendOpAlpha = s_BlendOperations[size_t(desc.AlphaOperation)];
        }

        rtBlendDesc.RenderTargetWriteMask = desc.ColorWrite ? D3D11_COLOR_WRITE_ENABLE_ALL : 0;
    }

    ID3D11BlendState *pD3DBlendState;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateBlendState(&d3dDesc, &pD3DBlendState), nullptr);

    pBlendState = new BlendState();
    pBlendState->m_pBlendState = pD3DBlendState;

    return pBlendState;
}

InputLayout* GfxDevice::CreateInputLayout(const InputLayoutDescriptor &desc, Shader *pInputSignatureShader)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements;
    InputLayout *pInputLayout = new InputLayout();

    ID3DBlob *pShaderBuffer = pInputSignatureShader->m_pShaderBuffer;
    size_t elementCount = desc.Elements.size();

    if (elementCount == 0 || pShaderBuffer == nullptr)
    {
        return pInputLayout;
    }

    for (size_t i = 0; i < elementCount; ++i)
    {
        const InputLayoutElement &element = desc.Elements[i];
        AddInputLayoutElement(element, inputLayoutElements);
    }

    ID3D11InputLayout *pD3DInputLayout = nullptr;

    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateInputLayout(
        inputLayoutElements.data(), 
        uint32_t(inputLayoutElements.size()), 
        pShaderBuffer->GetBufferPointer(), 
        uint32_t(pShaderBuffer->GetBufferSize()), 
        &pD3DInputLayout), pInputLayout);

    pInputLayout->m_pLayout = pD3DInputLayout;

    return pInputLayout;
}

RasterizerState* GfxDevice::CreateRasterizerState(const RasterizerStateDescriptor &desc)
{
    D3D11_RASTERIZER_DESC d3dDesc = {};
    d3dDesc.FillMode = D3D11_FILL_SOLID;
    d3dDesc.CullMode = s_CullModes[size_t(desc.CullMode)];
    d3dDesc.FrontCounterClockwise = desc.Winding == Winding::FrontCounterClockwise;
    d3dDesc.DepthClipEnable = desc.DepthClip;

    ID3D11RasterizerState *pD3DState = nullptr;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateRasterizerState(&d3dDesc, &pD3DState), nullptr);
    RasterizerState *pState = new RasterizerState();
    pState->m_pD3DRasterizerState = pD3DState;

    return pState;
}

DepthStencilState* GfxDevice::CreateDepthStencilState(const DepthStencilStateDescriptor &desc)
{
    D3D11_DEPTH_STENCIL_DESC d3dDesc;
    d3dDesc.DepthEnable = desc.IsDepthTestEnabled;
    d3dDesc.DepthFunc = s_ComparisonFuncs[size_t(desc.DepthFunction)];
    d3dDesc.DepthWriteMask = desc.IsDepthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    d3dDesc.StencilEnable = desc.IsStencilEnabled;
    d3dDesc.StencilReadMask = desc.StencilReadMask;
    d3dDesc.StencilWriteMask = desc.StencilWriteMask;
    d3dDesc.FrontFace.StencilDepthFailOp = s_StencilOps[size_t(desc.FrontFaceStencil.DepthFailOp)];
    d3dDesc.FrontFace.StencilFailOp = s_StencilOps[size_t(desc.FrontFaceStencil.StencilFailOp)];
    d3dDesc.FrontFace.StencilPassOp = s_StencilOps[size_t(desc.FrontFaceStencil.DepthStencilPassOp)];
    d3dDesc.FrontFace.StencilFunc = s_ComparisonFuncs[size_t(desc.FrontFaceStencil.Function)];
    d3dDesc.BackFace.StencilDepthFailOp = s_StencilOps[size_t(desc.BackFaceStencil.DepthFailOp)];
    d3dDesc.BackFace.StencilFailOp = s_StencilOps[size_t(desc.BackFaceStencil.StencilFailOp)];
    d3dDesc.BackFace.StencilPassOp = s_StencilOps[size_t(desc.BackFaceStencil.DepthStencilPassOp)];
    d3dDesc.BackFace.StencilFunc = s_ComparisonFuncs[size_t(desc.BackFaceStencil.Function)];

    ID3D11DepthStencilState *pD3DDepthStencilState;
    CHECK_HRESULT_RETURN_VALUE(m_pD3DDevice->CreateDepthStencilState(&d3dDesc, &pD3DDepthStencilState), nullptr);
    DepthStencilState *pDepthStencilState = new DepthStencilState();
    pDepthStencilState->m_pDepthStencilState = pD3DDepthStencilState;

    return pDepthStencilState;
}

void GfxDevice::AddInputLayoutElement(const InputLayoutElement &element, std::vector<D3D11_INPUT_ELEMENT_DESC> &elements)
{
    InputLayoutSemantic elementSemantic = element.Semantic;
    size_t szElement = size_t(elementSemantic);
    assert(szElement < size_t(InputLayoutSemantic::EnumCount));

    LPCSTR semantic = s_SemanticNames[szElement];
    uint32_t semanticIndex = GetInputLayoutSemanticIndex(elementSemantic, elements);
    DXGI_FORMAT format = s_InputLayoutFormats[szElement];
    uint32_t alignedByteOffset = GetInputLayoutElementAlignedByteOffset(elementSemantic, elements);

    elements.push_back({ semantic, semanticIndex, format, element.Slot, alignedByteOffset,  D3D11_INPUT_PER_VERTEX_DATA, 0 });
}

uint32_t GfxDevice::GetInputLayoutSemanticIndex(InputLayoutSemantic elementSemantic, const std::vector<D3D11_INPUT_ELEMENT_DESC> &elements)
{
    uint32_t index = 0;
    size_t elementCount = elements.size();

    for (size_t elementIndex = 0; elementIndex < elementCount; ++elementIndex)
    {
        const D3D11_INPUT_ELEMENT_DESC &elementDesc = elements[elementIndex];
        index = elementDesc.SemanticIndex > index ? elementDesc.SemanticIndex : index;
    }

    return index;
}

uint32_t GfxDevice::GetInputLayoutElementAlignedByteOffset(InputLayoutSemantic elementSemantic, const std::vector<D3D11_INPUT_ELEMENT_DESC> &elements)
{
    uint32_t offset = 0;
    size_t elementCount = elements.size();

    for (size_t elementIndex = 0; elementIndex < elementCount; ++elementIndex)
    {
        const D3D11_INPUT_ELEMENT_DESC& elementDesc = elements[elementIndex];

        switch (elementDesc.Format)
        {
        case DXGI_FORMAT_R32G32_FLOAT:
            offset += 8;
            break;
        case DXGI_FORMAT_R32G32B32_FLOAT:
            offset += 12;
            break;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            offset += 16;
            break;
        }
    }

    return offset;
}

NativePixelFormat GfxDevice::ConvertToNativePixelFormat(PixelFormat format)
{
    assert(size_t(format) < ARRAYSIZE(s_PixelFormats));
    return s_PixelFormats[size_t(format)];
}

