#pragma once

#include <stdafx.h>

namespace lwgl
{
    namespace resources
    {
        class Mesh;
    }

    namespace pipeline
    {
        class GfxPipeline;
    }

    using namespace resources;
    using namespace pipeline;

    namespace core
    {
        class GfxDeviceContext
        {
        public:

            GfxDeviceContext(ID3D11DeviceContext* d3dContext);
            ~GfxDeviceContext();

            void SetupPipeline(const GfxPipeline *pPipeline);
            void DrawMesh(Mesh *mesh);

        private:

            ID3D11DeviceContext* m_pD3DContext;
        };
    }
}