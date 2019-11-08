#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class Texture;

        class RenderTarget final : public RefCountedObject<RenderTarget>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            RenderTarget();
            ~RenderTarget();

        private:

            Texture*              m_pRenderTexture;
            ID3D11RenderTargetView* m_pRenderTargetView;
        };
    }
}