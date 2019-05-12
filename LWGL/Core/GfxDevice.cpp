#include "GfxDevice.h"
#include <stdafx.h>
#include "../3rd/DXUT/Optional/SDKmisc.h"

#include "../Descriptors/ShaderDescriptor.h"
#include "../Descriptors/BlendStateDescriptor.h"
#include "../Descriptors/InputLayoutDescriptor.h"
#include "../Descriptors/PixelFormats.h"
#include "../Descriptors/PipelineDescriptor.h"

#include "../Resources/Mesh.h"
#include "../Resources/Shader.h"
#include "../Resources/BlendState.h"
#include "../Resources/InputLayout.h"
#include "../Resources/DepthStencilState.h"
#include "../Resources/Texture2D.h"

#include "../Pipeline/GfxPipeline.h"

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
    "TEXCOORD",
    "TEXCOORD",
    "TEXCOORD",
    "TEXCOORD",
};

const DXGI_FORMAT GfxDevice::s_Formats[] =
{
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

GfxDevice::GfxDevice(ID3D11Device* d3dDevice)
    : m_pD3DDevice(d3dDevice)
{
    static_assert(ARRAYSIZE(s_PixelFormats) == size_t(PixelFormat::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_ShaderModels) == size_t(ShaderType::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_BlendValues) == size_t(BlendValue::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_BlendOperations) == size_t(BlendOperation::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_SemanticNames) == size_t(InputLayoutSemantic::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_Formats) == size_t(InputLayoutSemantic::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_StencilOps) == size_t(StencilOperation::EnumCount), "Array is missing values");
    static_assert(ARRAYSIZE(s_ComparisonFuncs) == size_t(ComparisonFunction::EnumCount), "Array is missing values");
}

GfxDevice::~GfxDevice()
{

}

GfxPipeline* GfxDevice::CreatePipeline(const PipelineDescriptor &desc)
{
    GfxPipeline *pPipeline = new GfxPipeline();

    pPipeline->m_pInputLayout = CreateInputLayout(desc.InputLayout);
    pPipeline->m_pVertexShader = CreateShader(desc.VertexShader);
    pPipeline->m_pFragmentShader = CreateShader(desc.FragmentShader);
    pPipeline->m_pBlendState = CreateBlendState(desc.BlendState);
    pPipeline->m_pDepthStencilState = CreateDepthStencilState(desc.DepthStencilState);

    return pPipeline;
}

Mesh* GfxDevice::CreateMesh(const wchar_t *filePath)
{
    Mesh* mesh = new Mesh();
    mesh->m_DXUTMesh.Create(m_pD3DDevice, filePath);
    return mesh;
}

Texture2D* GfxDevice::CreateTexture(const wchar_t *filePath)
{
    //HRESULT WINAPI DXUTCreateShaderResourceViewFromFile(_In_ ID3D11Device * d3dDevice, _In_z_ const wchar_t* szFileName, _Outptr_ ID3D11ShaderResourceView * *textureView);
    return nullptr;
}

Shader* GfxDevice::CreateShader(const ShaderDescriptor &desc)
{
    assert(desc.Type == ShaderType::VertexShader || desc.Type == ShaderType::FragmentShader);

    HRESULT hr;
    Shader *pShader = new Shader();;

    ShaderType type = desc.Type;
    wchar_t *filePath = desc.FilePath;
    char *entryPoint = desc.EntryPoint;
    const char *debugName = desc.DebugName;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    DWORD debugFlag = debugName != nullptr ? D3DCOMPILE_SKIP_OPTIMIZATION : 0;
    dwShaderFlags |= debugFlag;

    ID3DBlob* pShaderBuffer = nullptr;
    CHECK_HRESULT_BOOL(DXUTCompileFromFile(filePath, nullptr, entryPoint, s_ShaderModels[size_t(type)], dwShaderFlags, 0, &pShaderBuffer));
    pShader->m_pShaderBuffer = pShaderBuffer;

    if (type == ShaderType::VertexShader)
    {
        ID3D11VertexShader *pVertexShader;
        CHECK_HRESULT_BOOL(m_pD3DDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, &pVertexShader));

        pShader->m_pVertexShader = pVertexShader;
        pShader->m_Type = ShaderType::VertexShader;

        if (debugName != nullptr)
        {
            DXUT_SetDebugName(pVertexShader, debugName);
        }
    }
    else if (type == ShaderType::FragmentShader)
    {
        ID3D11PixelShader *pFragmentShader;
        CHECK_HRESULT_BOOL(m_pD3DDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, &pFragmentShader));

        pShader->m_pFragmentShader = pFragmentShader;
        pShader->m_Type = ShaderType::FragmentShader;

        if (debugName != nullptr)
        {
            DXUT_SetDebugName(pFragmentShader, debugName);
        }
    }

    return pShader;
}

BlendState* GfxDevice::CreateBlendState(const BlendStateDescriptor &desc)
{
    HRESULT hr;
    BlendState *pBlendState = nullptr;

    D3D11_BLEND_DESC d3dDesc;
    D3D11_RENDER_TARGET_BLEND_DESC &rtBlendDesc = d3dDesc.RenderTarget[0];
    d3dDesc.AlphaToCoverageEnable = false;
    d3dDesc.IndependentBlendEnable = true;
    
    if (rtBlendDesc.BlendEnable = desc.IsEnabled)
    {
        rtBlendDesc.SrcBlend                = s_BlendValues[size_t(desc.SourceColor)];
        rtBlendDesc.DestBlend               = s_BlendValues[size_t(desc.DestinationColor)];
        rtBlendDesc.BlendOp                 = s_BlendOperations[size_t(desc.ColorOperation)];
        rtBlendDesc.SrcBlendAlpha           = s_BlendValues[size_t(desc.SourceAlpha)];
        rtBlendDesc.DestBlendAlpha          = s_BlendValues[size_t(desc.DestinationAlpha)];
        rtBlendDesc.BlendOpAlpha            = s_BlendOperations[size_t(desc.AlphaOperation)];
        rtBlendDesc.RenderTargetWriteMask   = 255;
    }

    ID3D11BlendState *pD3DBlendState;
    CHECK_HRESULT_PTR(m_pD3DDevice->CreateBlendState(&d3dDesc, &pD3DBlendState));

    pBlendState = new BlendState();
    pBlendState->m_pBlendState = pD3DBlendState;

    return pBlendState;
}

InputLayout* GfxDevice::CreateInputLayout(const InputLayoutDescriptor &desc)
{
    HRESULT hr;

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements(desc.Elements.size());

    size_t elementCount = desc.Elements.size();
    for (int i = 0; i < elementCount; ++i)
    {
        const InputLayoutElement &element = desc.Elements[i];
        AddInputLayoutElement(element, inputLayoutElements);
    }

    ID3DBlob *pShaderBuffer = desc.InputSignatureShader->m_pShaderBuffer;
    ID3D11InputLayout *pD3DInputLayout = nullptr;

    CHECK_HRESULT_PTR(m_pD3DDevice->CreateInputLayout(
        inputLayoutElements.data(), 
        uint32_t(inputLayoutElements.size()), 
        pShaderBuffer->GetBufferPointer(), 
        uint32_t(pShaderBuffer->GetBufferSize()), 
        &pD3DInputLayout));

    InputLayout *pInputLayout = new InputLayout();
    pInputLayout->m_pLayout = pD3DInputLayout;

    return pInputLayout;
}

DepthStencilState* GfxDevice::CreateDepthStencilState(const DepthStencilStateDescriptor &desc)
{
    HRESULT hr;
    DepthStencilState *pDepthStencilState = new DepthStencilState();

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
    CHECK_HRESULT_PTR(m_pD3DDevice->CreateDepthStencilState(&d3dDesc, &pD3DDepthStencilState));
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
    DXGI_FORMAT format = s_Formats[szElement];
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

