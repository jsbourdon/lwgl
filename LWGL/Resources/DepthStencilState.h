#pragma once

#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class DepthStencilState : public RefCountedResource<DepthStencilState>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            DepthStencilState();
            ~DepthStencilState();

        private:

            ID3D11DepthStencilState *m_pDepthStencilState;
        };
    }
}