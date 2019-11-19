#pragma once

#include "LWGL_Common/Math/Math.h"

namespace lwgl
{
    using namespace math;

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