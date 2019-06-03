#pragma once

#include "Core/RefCountedObject.h"

namespace lwgl
{
    namespace core
    {
        class GfxDevice;
        class GfxDeviceContext;
    }

    namespace descriptors
    {
        struct SurfaceDescriptor;
    }

    using namespace core;
    using namespace descriptors;

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

        private:

            DebuggingFeatures();
            ~DebuggingFeatures();

            bool Init(GfxDevice *pDevice, GfxDeviceContext *pContext);
            bool OnSwapChainResized(GfxDevice *pDevice, const NativeSurfaceDescriptor *pSurfaceDesc);
            void OnSwapChainReleased();
            void OnUpdate(double fTime, float fElapsedTime);
            void OnFrameRender(double fTime, float fElapsedTime);

            void UpdateFPS(float fElapsedTimeSec);
            void RenderFPS();

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
    #endif
        };
    }
}