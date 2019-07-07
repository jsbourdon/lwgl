#pragma once

#include "Resources/Texture.h"

namespace lwgl
{
    namespace descriptors
    {
        enum class TextureType;
    }

    using namespace descriptors;

    namespace resources
    {
        class TextureArray final : public Texture
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            TextureArray() = default;
            ~TextureArray();

        private:

            union
            {
                ID3D11RenderTargetView**    m_pRTVs { nullptr };
                ID3D11DepthStencilView**    m_pDSVs;
            };

            size_t                          m_ArraySize { 0 };
        };
    }
}