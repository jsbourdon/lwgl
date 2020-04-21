#pragma once

#include "RefCountedObject.h"

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
        class Texture;
        class TextureArray;
        class SamplerState;
        class RasterizerState;
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
        struct TextureDescriptor;
        struct SamplerStateDescriptor;
        struct RasterizerStateDescriptor;
        enum class InputLayoutSemantic;
        enum class PixelFormat;
    }
    
    using namespace resources;
    using namespace descriptors;
    using namespace pipeline;

    namespace core
    {
        class GfxDevice : public RefCountedObject<GfxDevice>
        {
            friend base;

        public:

            GfxDevice(ID3D11Device* d3dDevice);

            GfxNativeDevice*    GetNativeDevice();

            GfxPipeline*        CreatePipeline(const PipelineDescriptor &desc);
            Mesh*               CreateMesh(const wchar_t *filePath);
            Buffer*             CreateBuffer(const BufferDescriptor &desc);
            Texture*            CreateTexture(const TextureDescriptor &desc);
            TextureArray*       CreateTextureArray(const TextureDescriptor &desc);
            SamplerState*       CreateSamplerState(const SamplerStateDescriptor &desc);
            Shader*             CreateShader(const ShaderDescriptor &desc);
            InputLayout*        CreateInputLayout(const InputLayoutDescriptor &desc, Shader *pInputSignatureShader);

            static NativePixelFormat    ConvertToNativePixelFormat(PixelFormat format);

        private:

            ~GfxDevice();

            BlendState*         CreateBlendState(const BlendStateDescriptor &desc);
            DepthStencilState*  CreateDepthStencilState(const DepthStencilStateDescriptor &desc);
            RasterizerState*    CreateRasterizerState(const RasterizerStateDescriptor &desc);

            template<typename T> T* CreateTextureCore(const TextureDescriptor &desc);
            Texture*                CreateTextureViews(Texture *pTexture, const TextureDescriptor &desc);
            TextureArray*           CreateTextureViews(TextureArray *pTexture, const TextureDescriptor &desc);

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
            static const uint32_t               s_CPUAccessFlags[];
            static const D3D11_USAGE            s_ResourceUsages[];
            static const D3D11_USAGE            s_TextureUsages[];
            static const D3D11_CULL_MODE        s_CullModes[];

            ID3D11Device*   m_pD3DDevice;
        };
    }
}