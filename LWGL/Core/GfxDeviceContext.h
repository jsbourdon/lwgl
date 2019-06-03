#pragma once

#include <pch.h>
#include "RefCountedObject.h"

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

        class GfxDeviceContext : public RefCountedObject<GfxDeviceContext>
        {
            friend base;

        public:

            GfxDeviceContext(ID3D11DeviceContext* d3dContext);

            GfxNativeDeviceContext* GetNativeContext();

            void    SetupPipeline(GfxPipeline *pPipeline);
            void    DrawMesh(Mesh *mesh);

            void*   MapBuffer(Buffer *pBuffer, MapType mapType);
            void    UnmapBuffer(Buffer *pBuffer);
            void    BindBuffer(const Buffer *pBuffer, Stage stage, uint32_t slot);

            void    BindSampler(SamplerState *pSampler, Stage stage, uint32_t slot);

            void    Clear(const ClearDescriptor &desc);

        private:

            ~GfxDeviceContext();

            void BindBufferToVSStage(const Buffer *pBuffer, uint32_t slot);
            void BindBufferToPSStage(const Buffer *pBuffer, uint32_t slot);

            static const D3D11_MAP  s_MapTypes[];

            ID3D11DeviceContext*    m_pD3DContext;
            GfxPipeline*            m_pCurrentPipeline;
        };
    }
}