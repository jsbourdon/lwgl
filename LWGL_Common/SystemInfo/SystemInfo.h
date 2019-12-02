#pragma once

#include <stdint.h>

namespace lwgl
{
    namespace system
    {
        enum class CPUArchitecture
        {
            Unsupported = -1,
            x64,
            ARM64,
            Count
        };

        struct SystemInfo
        {
            CPUArchitecture m_CpuArchitecture;
            uint32_t m_AllocationGranularity;
            uint32_t m_AllocationPageSize;
            uint32_t m_LogicalCpuCoreCount;
        };

        SystemInfo GetSystemInfo();
    }
}