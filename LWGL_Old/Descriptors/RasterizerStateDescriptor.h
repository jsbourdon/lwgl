#pragma once

namespace lwgl
{
    namespace descriptors
    {
        enum class CullMode
        {
            None,
            Front,
            Back,
            EnumCount
        };

        enum class Winding
        {
            FrontClockwise,
            FrontCounterClockwise,
            EnumCount
        };

        struct RasterizerStateDescriptor
        {
            CullMode    CullMode { CullMode::Back };
            Winding     Winding { Winding::FrontClockwise };
            bool        DepthClip { true };
        };
    }
}