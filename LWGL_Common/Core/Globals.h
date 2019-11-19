#pragma once

#include <cstdint>

namespace lwgl
{
    namespace core
    {
        template<typename Enum>
        inline Enum CombineFlags(Enum flag0, Enum flag1)
        {
            return static_cast<Enum>(size_t(flag0) | size_t(flag1));
        }
    }
}