#pragma once

namespace lwgl
{
    namespace core
    {
        class GfxDevice;
        class GfxDeviceContext;
        class RenderCore;

        class IRenderer
        {
        public:
            virtual bool Init(RenderCore *pRenderCore, GfxDevice* pDevice, GfxDeviceContext* pContext) = 0;
            virtual void Destroy(RenderCore *pRenderCore) = 0;
            virtual void OnUpdate(RenderCore *pRenderCore, GfxDeviceContext* pContext, double fTimeSec, float fElapsedTimeSec, void* pUserContext) = 0;
            virtual void OnFrameRender(RenderCore *pRenderCore, GfxDevice* pDevice, GfxDeviceContext* pContext, double fTimeSec, float fElapsedTimeSec, void* pUserContext) = 0;
        };
    }
}