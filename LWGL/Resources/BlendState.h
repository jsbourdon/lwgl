#pragma once

#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class BlendState : public RefCountedResource<BlendState>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            BlendState();
            ~BlendState();

        private:

            ID3D11BlendState* m_pBlendState;
        };
    }
}