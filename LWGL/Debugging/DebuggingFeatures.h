#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace core
    {
        class RenderCore;
        class GfxDevice;
        class GfxDeviceContext;
    }

    namespace descriptors
    {
        struct SurfaceDescriptor;
    }

    namespace utilities
    {
        class Camera;
    }

    using namespace core;
    using namespace descriptors;
    using namespace utilities;

    namespace debugging
    {
        class DebugResourceManager;

        class DebuggingFeatures : RefCountedObject<DebuggingFeatures>
        {
            friend base;
            friend class RenderCore;

        public:

            void DrawText(const wchar_t *pText, const RECT &pos, const Vector4 &color);
            void ShowFPS(bool show);
            void ShowMainCameraPosition(bool show);

        private:

            DebuggingFeatures();
            ~DebuggingFeatures();

            bool Init(GfxDevice *pDevice, GfxDeviceContext *pContext);
            bool OnSwapChainResized(GfxDevice *pDevice, const NativeSurfaceDescriptor *pSurfaceDesc);
            void OnSwapChainReleased();
            void OnUpdate(RenderCore *pCore, double fTime, float fElapsedTime);
            void OnFrameRender(RenderCore *pCore, double fTime, float fElapsedTime);

            void UpdateFPS(float fElapsedTimeSec);
            void RenderFPS();
            void RenderCameraPosition(const Camera *pCamera);

        private:

            static constexpr float s_FPSUpdateRateSec = 0.5f;

    #ifdef _DEBUG
            DebugResourceManager*   m_pRscManager;
            GfxDevice*              m_pDevice;
            GfxDeviceContext*       m_pContext;
            float                   m_AverageFPS;
            float                   m_AccumulatedFPSElapsedTimeSec;
            uint32_t                m_AccumulatedFPSFrames;
            bool                    m_ShowFPS;
            bool                    m_ShowCameraPos;
    #endif
        };
    }
}