#pragma once

#include <stdafx.h>

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

        enum class BufferUsage
        {
            GPU_ReadWrite,
            GPU_ReadOnly,
            GPU_ReadOnly_CPU_WriteOnly,
            GPU_WriteOnly_CPU_ReadOnly,
            EnumCount
        };

        struct BufferDescriptor
        {
            const char*     DebugName;
            size_t          ByteSize;
            BufferType      Type;
            BufferUsage     Usage;
            uint32_t        StructureStride;
        };
    }
}