#pragma once

#include <pch.h>

namespace lwgl
{
    using namespace core;

    namespace descriptors
    {
        struct ClearDescriptor
        {
            Vector4     ColorClearValue;
            float       DepthClearValue;
            uint8_t     StencilClearValue;
            bool        ClearColor;
            bool        ClearDepth;
            bool        ClearStencil;
        };
    }
}