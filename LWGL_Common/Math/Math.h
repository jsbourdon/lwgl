#pragma once

#include "../3rd/DirectXMath/Inc/DirectXMath.h"

namespace lwgl
{
    namespace math
    {
        typedef DirectX::XMMATRIX       Matrix4x4;
        typedef DirectX::XMFLOAT3X3     Matrix3x3;
        typedef DirectX::XMVECTORF32    Vector4;
        typedef DirectX::XMFLOAT3       Vector3;
        typedef DirectX::XMFLOAT2       Vector2;

        inline Matrix4x4 InverseMatrix(Matrix4x4 &matrix)
        {
            return XMMatrixInverse(nullptr, matrix);
        }

        inline Vector3 ConvertVector(const Vector4 &vec4)
        {
            Vector3 vec3;
            XMStoreFloat3(&vec3, vec4);
            return vec3;
        }

        inline Vector4 ConvertVector(const Vector3 &vec3)
        {
            DirectX::XMVECTOR xmVector = DirectX::XMLoadFloat3(&vec3);
            return *reinterpret_cast<Vector4*>(&xmVector);
        }

        inline Vector4 GetMatrixRow(Matrix4x4 &matrix, uint32_t rowIndex)
        {
            assert(rowIndex < 4);
            return *reinterpret_cast<Vector4*>(&matrix.r[rowIndex]);
        }

        inline Vector2 GetViewSpaceZParams(float nearClip, float farClip)
        {
            float rcpNear = 1.0f / nearClip;
            float rcpFar = 1.0f / farClip;
            return Vector2 { (rcpFar - rcpNear), rcpNear };
        }

        template<typename T>
        inline constexpr T Log2(T value)
        {
            return (value < 2) ? 1 : Log2(value >> 1);
        }

        template<typename T>
        inline constexpr T CountTrailingZeros(T value)
        {
            return Log2(value & -value);
        }

        template<typename T>
        inline constexpr T CountLeadingZeros(T value)
        {
            return sizeof(value) * 8 - CountTrailingZeros(0x01 << Log2(value));
        }
    }
}