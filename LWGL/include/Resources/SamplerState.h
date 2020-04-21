#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class SamplerState final : public RefCountedObject<SamplerState>
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