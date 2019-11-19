#pragma once

#include "ResourceUsage.h"

namespace lwgl
{
    namespace descriptors
    {
        enum class BufferType
        {
            Constants,
            Structured,
            ReadWrite,
            Vertex,
            Index,
            EnumCount
        };

        struct BufferDescriptor
        {
            const char*     DebugName;
            size_t          ByteSize;
            size_t          StructureStride;
            BufferType      Type;
            ResourceUsage   Usage;
        };
    }
}