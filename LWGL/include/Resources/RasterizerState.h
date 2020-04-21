#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class RasterizerState final : public RefCountedObject<RasterizerState>
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