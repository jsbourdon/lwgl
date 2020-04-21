#pragma once

namespace lwgl
{
    namespace descriptors
    {
        enum class PixelFormat;

        struct SurfaceDescriptor
        {
            uint32_t    Width;
            uint32_t    Height;
            uint32_t    SampleCount;
            PixelFormat Format;
        };
    }
}