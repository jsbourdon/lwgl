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
        class Mesh : public RefCountedObject<Mesh>
        {
        private:

            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

            Mesh();
            ~Mesh();

            CDXUTSDKMesh m_DXUTMesh;
        };
    }
}