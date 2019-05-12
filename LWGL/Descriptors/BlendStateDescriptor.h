#pragma once

namespace lwgl
{
    namespace descriptors
    {
        enum class BlendValue
        {
            ZERO,
            ONE,
            SRC_COLOR,
            INV_SRC_COLOR,
            SRC_ALPHA,
            INV_SRC_ALPHA,
            DEST_ALPHA,
            INV_DEST_ALPHA,
            DEST_COLOR,
            INV_DEST_COLOR,
            EnumCount
        };

        enum class BlendOperation
        {
            ADD,
            SUBTRACT,
            REV_SUBTRACT,
            MIN,
            MAX,
            EnumCount
        };

        struct BlendStateDescriptor
        {
            bool IsEnabled;
            BlendValue SourceColor;
            BlendValue DestinationColor;
            BlendOperation ColorOperation;
            BlendValue SourceAlpha;
            BlendValue DestinationAlpha;
            BlendOperation AlphaOperation;
        };
    }
}
