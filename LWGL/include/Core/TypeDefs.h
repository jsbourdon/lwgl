#pragma once

#include "3rd/DXUT/Core/DXUT.h"

namespace lwgl
{
    namespace core
    {
        typedef DirectX::XMMATRIX       Matrix4x4;
        typedef DirectX::XMFLOAT3X3     Matrix3x3;
        typedef DirectX::XMVECTORF32    Vector4;
        typedef DirectX::XMFLOAT3       Vector3;
        typedef DirectX::XMFLOAT2       Vector2;
        typedef ID3D11Device            GfxNativeDevice;
        typedef ID3D11DeviceContext     GfxNativeDeviceContext;
        typedef DXGI_FORMAT             NativePixelFormat;
        typedef DXGI_SURFACE_DESC       NativeSurfaceDescriptor;
    }
}