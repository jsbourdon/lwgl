#pragma once

#include <cstdint>

namespace lwgl
{
    namespace descriptors
    {
        struct ScissorRectDescriptor
        {
            uint32_t m_Left;
            uint32_t m_Top;
            uint32_t m_Right;
            uint32_t m_Bottom;
        };
    }
}
