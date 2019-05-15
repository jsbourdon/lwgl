#pragma once

#include <stdafx.h>

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

        class GfxDeviceContext
        {
        public:

            GfxDeviceContext(ID3D11DeviceContext* d3dContext);
            ~GfxDeviceContext();

            void    SetupPipeline(const GfxPipeline *pPipeline);
            void    DrawMesh(Mesh *mesh);

            void*   MapBuffer(Buffer *pBuffer, MapType mapType);
            void    UnmapBuffer(Buffer *pBuffer);
            void    BindBuffer(Buffer *pBuffer, Stage stage, uint32_t slot);

            void    BindSampler(SamplerState *pSampler, Stage stage, uint32_t slot);

            void    Clear(const ClearDescriptor &desc);

        private:

            static const D3D11_MAP s_MapTypes[];

            ID3D11DeviceContext* m_pD3DContext;
        };
    }
}