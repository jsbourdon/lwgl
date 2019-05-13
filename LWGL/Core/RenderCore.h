#pragma once

#include <stdafx.h>
#include <vector>
#include "IRenderer.h"

namespace lwgl
{
    namespace utilities
    {
        class Camera;
    }

    using namespace utilities;

    namespace core
    {
        class RenderCore
        {

        public:

            RenderCore(IRenderer *renderer);
            ~RenderCore();

            void        Init(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight);
            Camera*     CreateCamera(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane);
            void        StartRenderLoop();

        private:

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

            IRenderer* m_Renderer;
            GfxDevice* m_Device;
            GfxDeviceContext* m_DeviceContext;
            std::vector<Camera*> m_Cameras;
        };

    }
}

