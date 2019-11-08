#include <pch.h>

#include "RenderCore.h"
#include "IRenderer.h"
#include "GfxDevice.h"
#include "GfxDeviceContext.h"
#include "Utilities/Camera.h"
#include "Inputs/IInputReceiver.h"
#include "Debugging/DebuggingFeatures.h"
#include "Descriptors/TextureDescriptor.h"
#include "Descriptors/ShaderDescriptor.h"
#include "Descriptors/InputLayoutDescriptor.h"

#include <string>

using namespace lwgl;
using namespace core;

static const char* s_FullScreenTriangleVSCode 
{
    #include "InlinedShaders/FullScreenTriangleVS.hlsli"
};

RenderCore* RenderCore::CreateCore()
{
    RenderCore *pRenderCore = new RenderCore();
    pRenderCore->AddRef(); // Final ref is to be held by DXUT

    return pRenderCore;
}

RenderCore::RenderCore()
    : m_pRenderer(nullptr)
    , m_pDevice(nullptr)
    , m_pDeviceContext(nullptr)
    , m_pReceiver(nullptr)
    , m_pDebuggingFeatures(new DebuggingFeatures())
    , m_BackBufferWidth(0)
    , m_BackBufferHeight(0)
{
    
}

RenderCore::~RenderCore()
{
    size_t cameraCount = m_Cameras.size();
    for (size_t i = 0; i < cameraCount; ++i)
    {
        m_Cameras[i]->Release();
    }

    m_pRenderer->~IRenderer();
    FreeAlignedAlloc(m_pRenderer);
    m_pRenderer = nullptr;

    SAFE_RELEASE(m_pDebuggingFeatures);
    SAFE_RELEASE(m_pDeviceContext);
    SAFE_RELEASE(m_pDevice);
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK RenderCore::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    std::vector<Camera*> &cameras = pCore->m_Cameras;

    size_t cameraCount = cameras.size();
    for (size_t i = 0; i < cameraCount; ++i)
    {
        cameras[i]->HandleMessages(hWnd, uMsg, wParam, lParam);
    }

    switch (uMsg)
    {
    case WM_GETMINMAXINFO:
        // override DXUT_MIN_WINDOW_SIZE_X and DXUT_MIN_WINDOW_SIZE_Y
        // to prevent windows that are too small
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 512;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 512;
        *pbNoFurtherProcessing = true;
        break;
    case WM_KEYDOWN:
    {
        size_t flags = static_cast<size_t>(lParam);
        bool firstDown = !((flags >> 30) & 0x01);
        pCore->OnKeyPressed(static_cast<uint32_t>(wParam), true, firstDown);
        break;
    }
    case WM_KEYUP:
        pCore->OnKeyPressed(static_cast<uint32_t>(wParam), false);
        break;
    }

    return 0;
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK RenderCore::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo* AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* DeviceInfo,
    DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
    return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RenderCore::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    IRenderer* pRenderer = pCore->m_pRenderer;
    ID3D11DeviceContext *pd3dContext = DXUTGetD3D11DeviceContext();

    GfxDevice *pDevice = new GfxDevice(pd3dDevice);
    GfxDeviceContext *pContext = new GfxDeviceContext(pd3dContext);
    pCore->m_pDevice = pDevice;
    pCore->m_pDeviceContext = pContext;

    if (!pCore->m_pDebuggingFeatures->Init(pDevice, pContext))
    {
        return E_FAIL;
    }

    pRenderer->Init(pCore, pDevice, pContext);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RenderCore::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    if (!pCore->m_pDebuggingFeatures->OnSwapChainResized(pCore->m_pDevice, pBackBufferSurfaceDesc))
    {
        return E_FAIL;
    }

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11ReleasingSwapChain(void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    pCore->m_pDebuggingFeatures->OnSwapChainReleased();
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11DestroyDevice(void* pUserContext)
{
    DXUTGetGlobalResourceCache().OnDestroyDevice();
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    IRenderer* pRenderer = pCore->m_pRenderer;
    pRenderer->Destroy(pCore);

    pCore->Release();
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);
    std::vector<Camera*> &cameras = pCore->m_Cameras;

    size_t cameraCount = cameras.size();
    for (size_t i = 0; i < cameraCount; ++i)
    {
        cameras[i]->FrameMove(fElapsedTime);
    }

    IRenderer* pRenderer = pCore->m_pRenderer;
    pRenderer->OnUpdate(pCore, pCore->m_pDeviceContext, fTime, fElapsedTime, nullptr);
    pCore->m_pDebuggingFeatures->OnUpdate(pCore, fTime, fElapsedTime);
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
    float fElapsedTime, void* pUserContext)
{
    RenderCore* pCore = static_cast<RenderCore*>(pUserContext);

    IRenderer* pRenderer = pCore->m_pRenderer;
    pRenderer->OnFrameRender(pCore, pCore->m_pDevice, pCore->m_pDeviceContext, fTime, fElapsedTime, nullptr);
    pCore->m_pDebuggingFeatures->OnFrameRender(pCore, fTime, fElapsedTime);
}

void RenderCore::InternalInit(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight)
{
    m_BackBufferWidth = windowWidth;
    m_BackBufferHeight = windowHeight;

    // Set DXUT callbacks
    DXUTSetCallbackMsgProc(MsgProc, this);
    DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable, this);
    DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice, this);
    DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain, this);
    DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain, this);
    DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice, this);

    DXUTSetCallbackFrameMove(OnFrameMove, this);
    DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender, this);

    DXUTInit(true, true, NULL); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings(true, true);
    DXUTCreateWindow(windowTitle);

    DXUTDeviceSettings deviceSettings;
    DXUTApplyDefaultDeviceSettings(&deviceSettings);
    deviceSettings.MinimumFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    deviceSettings.d3d11.sd.BufferDesc.Height = windowHeight;
    deviceSettings.d3d11.sd.BufferDesc.Width = windowWidth;
    deviceSettings.d3d11.sd.Windowed = TRUE;
    deviceSettings.d3d11.AutoCreateDepthStencil = false;

    DXUTCreateDeviceFromSettings(&deviceSettings, true);
    CreateDepthStencil(windowWidth, windowHeight);
    CreateFullScreenTriangleResources();

    m_pDeviceContext->BindSwapChain(true);
}

void RenderCore::CreateDepthStencil(uint32_t windowWidth, uint32_t windowHeight)
{
    TextureDescriptor desc;
    desc.Type = TextureType::DepthStencil;
    desc.BindFlags = TextureBindFlags::ShaderResource;
    desc.Format = PixelFormat::R24_UNORM_X8_TYPELESS;
    desc.Usage = ResourceUsage::GPU_ReadWrite;
    desc.Width = windowWidth;
    desc.Height = windowHeight;
    desc.ArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleCount = 1;

    m_pDeviceContext->SetSwapChainDepthStencil(m_pDevice->CreateTexture(desc));
}

void RenderCore::CreateFullScreenTriangleResources()
{
    ShaderDescriptor desc;
    desc.DebugName = "FullScreenTriangleVS";
    desc.EntryPoint = "VSMain";
    desc.Code = s_FullScreenTriangleVSCode;
    desc.Type = ShaderType::VertexShader;

    Shader *pShader = m_pDevice->CreateShader(desc);
    assert(pShader != nullptr);

    InputLayoutDescriptor inputDesc;
    inputDesc.Elements.push_back({ InputLayoutSemantic::Position, 0 });
    inputDesc.Elements.push_back({ InputLayoutSemantic::UV, 0 });
    InputLayout *pLayout = m_pDevice->CreateInputLayout(inputDesc, pShader);
    assert(pLayout != nullptr);

    m_pDeviceContext->SetFullScreenTriangleResources(pShader, pLayout);
}

void RenderCore::RegisterInputReceiver(IInputReceiver *pReceiver)
{
    m_pReceiver = pReceiver;
}

void RenderCore::OnKeyPressed(uint32_t keyCode, bool keyDown, bool firstDown)
{
    if (m_pReceiver != nullptr)
    {
        if (keyDown)
        {
            m_pReceiver->OnKeyDown(keyCode, firstDown);
        }
        else
        {
            m_pReceiver->OnKeyUp(keyCode);
        }
    }
}

void RenderCore::StartRenderLoop()
{
    DXUTMainLoop();
}

Camera* RenderCore::CreateCamera(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    Camera *pCamera = new Camera();
    pCamera->AddRef();
    pCamera->Init(worldPosition, lookAtWorldPosition, fov, aspectRatio, nearPlane, farPlane);
    m_Cameras.push_back(pCamera);

    return pCamera;
}

void RenderCore::GetBackBufferPixelSize(uint32_t &width, uint32_t &height)
{
    width = m_BackBufferWidth;
    height = m_BackBufferHeight;
}
