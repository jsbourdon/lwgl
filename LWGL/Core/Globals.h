#pragma once

#include "TypeDefs.h"

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

    namespace math
    {
        inline core::Matrix4x4 InverseMatrix(core::Matrix4x4 &matrix)
        {
            return XMMatrixInverse(nullptr, matrix);
        }

        inline core::Vector3 ConvertVector(const core::Vector4 &vec4)
        {
            core::Vector3 vec3;
            XMStoreFloat3(&vec3, vec4);
            return vec3;
        }

        inline core::Vector4 ConvertVector(const core::Vector3 &vec3)
        {
            DirectX::XMVECTOR xmVector = DirectX::XMLoadFloat3(&vec3);
            return *reinterpret_cast<core::Vector4*>(&xmVector);
        }

        inline core::Vector4 GetMatrixRow(core::Matrix4x4 &matrix, uint32_t rowIndex)
        {
            assert(rowIndex < 4);
            return *reinterpret_cast<core::Vector4*>(&matrix.r[rowIndex]);
        }

        inline core::Vector2 GetViewSpaceZParams(float nearClip, float farClip)
        {
            float rcpNear = 1.0f / nearClip;
            float rcpFar = 1.0f / farClip;
            return core::Vector2 { (rcpFar - rcpNear), rcpNear };
        }
    }
}