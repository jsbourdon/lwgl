#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class Texture2D;

        class RenderTarget : public RefCountedObject<RenderTarget>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            RenderTarget();
            ~RenderTarget();

        private:

            Texture2D*              m_pRenderTexture;
            ID3D11RenderTargetView* m_pRenderTargetView;
        };
    }
}