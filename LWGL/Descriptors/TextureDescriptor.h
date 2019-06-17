#pragma once

#include <pch.h>
#include "PixelFormats.h"
#include "ResourceUsage.h"

namespace lwgl
{
    namespace descriptors
    {
        enum class TextureType
        {
            Texture2D,
            EnumCount
        };

        enum class TextureBindFlags
        {
            ShaderResource      = 0x01,
            RenderTarget        = 0x02,
            UnorderedAccessView = 0x04,
        };

        struct TextureDescriptor
        {
            TextureType Type;
            PixelFormat Format;
            ResourceUsage Usage;
            TextureBindFlags BindFlags;
            uint32_t Width;
            uint32_t Height;
            uint32_t MipLevels;
            uint32_t ArraySize;
            uint32_t SampleCount;
        };
    }
}