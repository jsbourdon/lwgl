#pragma once

namespace lwgl
{
    enum class GfxPlatform
    {
        Unknown = -1,
        D3D11,
        D3D12,
        Vulkan,
        MoltenVK,
        NX,
        Neo,
        Scorpio,
        Prospero,
        Lockhart,
        Anaconda,
        Count
    };
}