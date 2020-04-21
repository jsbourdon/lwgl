#pragma once

#include <vector>

namespace lwgl
{
    namespace resources
    {
        class Shader;
    }

    namespace descriptors
    {
        enum class InputLayoutSemantic
        {
            Position,
            Normal,
            Tangent,
            UV,
            Float2,
            Float3,
            Float4,
            EnumCount
        };

        struct InputLayoutElement
        {
            InputLayoutSemantic Semantic;
            uint32_t            Slot;
        };

        struct InputLayoutDescriptor
        {
            std::vector<InputLayoutElement> Elements;
        };
    }
}