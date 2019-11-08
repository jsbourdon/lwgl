#pragma once

#include <pch.h>
#include "../3rd/DXUT/Optional/SDKmesh.h"
#include "../Core/RefCountedObject.h"

namespace lwgl
{
    namespace core
    {
        class GfxDevice;
        class GfxDeviceContext;
    }

    using namespace core;

    namespace resources
    {
        class Mesh final : public RefCountedObject<Mesh>
        {
        private:

            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

            Mesh() = default;
            ~Mesh();

            CDXUTSDKMesh m_DXUTMesh {};
            uint32_t m_AlbedoSlot { uint32_t(-1) };
        };
    }
}