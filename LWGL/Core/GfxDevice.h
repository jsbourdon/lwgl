#pragma once

#include <stdafx.h>

namespace lwgl
{
    namespace pipeline
    {
        class GfxPipeline;
    }

    namespace resources
    {
        class Mesh;
        class Buffer;
        class Shader;
        class BlendState;
        class InputLayout;
        class DepthStencilState;
        class Texture2D;
    }

    namespace descriptors
    {
        struct ShaderDescriptor;
        struct BlendStateDescriptor;
        struct DepthStencilStateDescriptor;
        struct PipelineDescriptor;
        struct InputLayoutDescriptor;
        struct InputLayoutElement;
        struct BufferDescriptor;
        enum class InputLayoutSemantic;
    }
    
    using namespace resources;
    using namespace descriptors;
    using namespace pipeline;

    namespace core
    {
        class GfxDevice
        {
        public:

            GfxDevice(ID3D11Device* d3dDevice);
            ~GfxDevice();

            GfxPipeline*    CreatePipeline(const PipelineDescriptor &desc);
            Mesh*           CreateMesh(const wchar_t *filePath);
            Buffer*         CreateBuffer(const BufferDescriptor &desc);
            Texture2D*      CreateTexture(const wchar_t *filePath);

        private:

            Shader*             CreateShader(const ShaderDescriptor &desc);
            BlendState*         CreateBlendState(const BlendStateDescriptor &desc);
            InputLayout*        CreateInputLayout(const InputLayoutDescriptor &desc, Shader *pInputSignatureShader);
            DepthStencilState*  CreateDepthStencilState(const DepthStencilStateDescriptor &desc);

            static void         AddInputLayoutElement(const InputLayoutElement &element, std::vector<D3D11_INPUT_ELEMENT_DESC> &elements);
            static uint32_t     GetInputLayoutSemanticIndex(InputLayoutSemantic elementSemantic, const std::vector<D3D11_INPUT_ELEMENT_DESC> &elements);
            static uint32_t     GetInputLayoutElementAlignedByteOffset(InputLayoutSemantic elementSemantic, const std::vector<D3D11_INPUT_ELEMENT_DESC> &elements);

        private:

            static const char *const            s_ShaderModels[];
            static const DXGI_FORMAT            s_PixelFormats[];
            static const D3D11_BLEND            s_BlendValues[];
            static const D3D11_BLEND_OP         s_BlendOperations[];
            static const LPCSTR                 s_SemanticNames[];
            static const DXGI_FORMAT            s_InputLayoutFormats[];
            static const D3D11_STENCIL_OP       s_StencilOps[];
            static const D3D11_COMPARISON_FUNC  s_ComparisonFuncs[];
            static const uint32_t               s_BufferBindFlags[];
            static const uint32_t               s_BufferCPUAccessFlags[];
            static const D3D11_USAGE            s_BufferUsages[];

            ID3D11Device*   m_pD3DDevice;
        };
    }
}