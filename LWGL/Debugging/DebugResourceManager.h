#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace descriptors
    {
        enum class PixelFormat;
        struct SurfaceDescriptor;
    }

    using namespace core;
    using namespace descriptors;

    namespace debugging
    {
        class DebugResourceManager : public RefCountedObject<DebugResourceManager>
        {
            friend base;
            friend class DebuggingFeatures;

            void SetupDrawTextResources(GfxNativeDeviceContext *pContext);
            void CleanDrawTextResources(GfxNativeDevice *pDevice, GfxNativeDeviceContext *pContext);

            void GetBackBufferSize(uint32_t &width, uint32_t &height);

        private:

            DebugResourceManager() = default;
            ~DebugResourceManager();

            bool Init(GfxNativeDevice *pDevice, GfxNativeDeviceContext *pContext);
            bool OnSwapChainResized(GfxNativeDevice *pDevice, const NativeSurfaceDescriptor *pSurfaceDesc);
            void OnSwapChainReleased();

        public:



        private:

            CDXUTDialogResourceManager  m_DXUTRscManager {};
        };
    }
}