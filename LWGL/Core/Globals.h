#pragma once

namespace lwgl
{
    namespace core
    {
        // MSFT: aligned_alloc() will probably never be implemented, as C11 specified it in a way that’s incompatible 
        // with our implementation(namely, that free() must be able to handle highly aligned allocations).
        inline void* AlignedAlloc(size_t size, size_t alignment)
        {
            return _aligned_malloc(size, alignment);
        }

        inline void FreeAlignedAlloc(void *pAlloc)
        {
            _aligned_free(pAlloc);
        }

        inline void* StackAlloc(size_t size)
        {
            return _malloca(size);
        }

        template<typename Enum>
        inline Enum CombineFlags(Enum flag0, Enum flag1)
        {
            return static_cast<Enum>(size_t(flag0) | size_t(flag1));
        }

        const Matrix4x4 s_IdentityMatrix =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
}