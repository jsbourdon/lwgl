#pragma once

#include "../Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class DepthStencilState : public RefCountedObject<DepthStencilState>
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