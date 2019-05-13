#pragma once

#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class RasterizerState : public RefCountedResource<RasterizerState>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            RasterizerState();
            ~RasterizerState();

        private:

            ID3D11RasterizerState*  m_pD3DRasterizerState;
        };
    }
}