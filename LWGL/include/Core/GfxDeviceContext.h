#pragma once

#include "Core/Constants.h"
#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace descriptors
    {
        struct ClearDescriptor;
    }

    namespace resources
    {
        class Mesh;
        class Buffer;
        class SamplerState;
        class Texture;
        class TextureArray;
        class Shader;
        class InputLayout;
    }

    namespace pipeline
    {
        class GfxPipeline;
    }

    using namespace descriptors;
    using namespace resources;
    using namespace pipeline;

    namespace core
    {
        enum class Stage
        {
            IA,
            VS,
            RS,
            PS,
            OM
        };

        enum class MapType
        {
            Read,
            Write,
            ReadWrite,
            WriteDiscard,
            WriteNoOverwrite
        };

        class GfxDeviceContext : public RefCountedObject<GfxDeviceContext>
        {
            friend base;

        public:

            GfxDeviceContext(ID3D11DeviceContext* d3dContext);

            GfxNativeDeviceContext* GetNativeContext();

            void    SetupPipeline(GfxPipeline *pPipeline);
            void    DrawMesh(Mesh *mesh);
            void    DrawFullScreenTriangle();

            void*   MapBuffer(Buffer *pBuffer, MapType mapType);
            void    UnmapBuffer(Buffer *pBuffer);

            void    BindBuffer(const Buffer *pBuffer, Stage stage, uint32_t slot);
            void    BindTexture(const Texture *pTexture, Stage stage, uint32_t slot);
            void    BindSampler(SamplerState *pSampler, Stage stage, uint32_t slot);
            void    BindSwapChainDepthStencilToStage(Stage stage, uint32_t slot);
            void    BindSwapChain(bool bindDepthStencil = true);
            void    Unbind(Stage stage, uint32_t slot);
            void    UnbindRange(Stage stage, uint32_t slot, uint32_t count);

            void    BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, bool bindDepthStencil = true);
            void    BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, Texture *pDepthBuffer);
            void    BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, TextureArray *pDepthBuffer, uint32_t depthArrayIndex);
            void    BindRenderTargets(TextureArray *pRenderTargets, uint32_t rtStartIndex, uint32_t renderTargetCount, TextureArray *pDepthBuffer, uint32_t depthArrayIndex);
            void    BindRenderTargets(Texture *pRenderTargets[], uint32_t renderTargetCount, ID3D11DepthStencilView *pDepthBuffer);

            void    SetViewport(float width, float height);
            void    Clear(const ClearDescriptor &desc);

            void    SetSwapChainDepthStencil(Texture *pDepthStencil);
            void    SetFullScreenTriangleResources(Shader *pShader, InputLayout *pLayout);

        private:

            ~GfxDeviceContext();

            void BindBufferToVSStage(const Buffer *pBuffer, uint32_t slot);
            void BindBufferToPSStage(const Buffer *pBuffer, uint32_t slot);
            void UnbindRenderTargets();
            void SetDepthStencil(Texture *pTexture, uint32_t arrayIndex = 0);
            void SetViewport(Texture *pTexture);

            static const D3D11_MAP  s_MapTypes[];
            static void*            s_pNullResources[lwgl::core::MAX_SHADERRESOURCE_COUNT];

            ID3D11DeviceContext*    m_pD3DContext { nullptr };
            GfxPipeline*            m_pCurrentPipeline { nullptr };
            Texture*                m_pRenderTargets[lwgl::core::MAX_RENDERTARGET_COUNT] {};
            Texture*                m_pSwapChainDepthStencil { nullptr };
            Texture*                m_pCurrentDepthStencil { nullptr };
            Shader*                 m_pFullScreenTriangleVS { nullptr };
            InputLayout*            m_pFullScreenTiangleInputLayout { nullptr };
            uint32_t                m_RenderTargetCount { 0 };
            uint32_t                m_DepthStencilArrayIndex { 0 };
        };
    }
}