#pragma once

#include <pch.h>
#include <vector>
#include "RefCountedObject.h"
#include "IRenderer.h"

namespace lwgl
{
    namespace utilities
    {
        class Camera;
    }

    namespace input
    {
        class IInputReceiver;
    }

    namespace debugging
    {
        class DebuggingFeatures;
    }

    using namespace utilities;
    using namespace input;
    using namespace debugging;

    namespace core
    {
        class GfxDevice;
        class GfxDeviceContext;

        class RenderCore : public RefCountedObject<RenderCore>
        {
            friend base;

        public:

            static RenderCore* CreateCore();

            template<typename T>
            void                        Init(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight);

            void                        RegisterInputReceiver(IInputReceiver *pReceiver);
            Camera*                     CreateCamera(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane);
            void                        StartRenderLoop();
            const std::vector<Camera*>& GetRegisteredCameras() const { return m_Cameras; }
            DebuggingFeatures*          GetDebuggingFeatures() { return m_pDebuggingFeatures; }
            void                        GetBackBufferPixelSize(uint32_t &width, uint32_t &height);

        private:

            RenderCore();
            ~RenderCore();

            static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
            static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
            static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
            static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
            static void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
            static void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, 
                double fTime, float fElapsedTime, void* pUserContext);
            static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo* AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* DeviceInfo,
                DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);
            static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

            void InternalInit(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight);
            void CreateDepthStencil(uint32_t windowWidth, uint32_t windowHeight);
            void OnKeyPressed(uint32_t keyCode, bool keyDown, bool firstDown = false);

            IRenderer*                  m_pRenderer;
            GfxDevice*                  m_pDevice;
            GfxDeviceContext*           m_pDeviceContext;
            IInputReceiver*             m_pReceiver;
            DebuggingFeatures*          m_pDebuggingFeatures;
            std::vector<Camera*>        m_Cameras;
            uint32_t                    m_BackBufferWidth;
            uint32_t                    m_BackBufferHeight;
        };

        template<typename T>
        void RenderCore::Init(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight)
        {
            void* alignedMem = AlignedAlloc(sizeof(T), alignof(T));
            m_pRenderer = new(alignedMem) T();
            InternalInit(windowTitle, windowWidth, windowHeight);
        }
    }
}

