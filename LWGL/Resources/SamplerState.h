#pragma once

#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class SamplerState : public RefCountedResource<SamplerState>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            SamplerState();
            ~SamplerState();

            ID3D11SamplerState* m_pSamplerState;
        };
    }
}