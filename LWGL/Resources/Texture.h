#pragma once

#include "Core/RefCountedObject.h"
#include "Descriptors/TextureDescriptor.h"

namespace lwgl
{
    using namespace descriptors;

    namespace resources
    {
        class Texture : public RefCountedObject<Texture>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            Texture() = default;
            ~Texture();

        private:

            ID3D11Texture2D*            m_pTexture { nullptr };
            ID3D11ShaderResourceView*   m_pSRV { nullptr };

            union
            {
                ID3D11RenderTargetView*     m_pRTV { nullptr };
                ID3D11DepthStencilView*     m_pDSV;
            };

            TextureType                 m_Type { TextureType::Texture2D };
        };
    }
}