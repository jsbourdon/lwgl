#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class BlendState final : public RefCountedObject<BlendState>
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