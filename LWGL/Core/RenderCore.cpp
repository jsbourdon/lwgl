#include "RenderCore.h"
#include "GfxDevice.h"
#include "GfxDeviceContext.h"
#include "Utilities/Camera.h"

using namespace lwgl;
using namespace core;

RenderCore::RenderCore()
    : m_pRenderer(nullptr)
    , m_pDevice(nullptr)
    , m_pDeviceContext(nullptr)
{
}

RenderCore::~RenderCore()
{
    size_t cameraCount = m_Cameras.size();
    for (int i = 0; i < cameraCount; ++i)
    {
        m_Cameras[i]->Release();
    }

    delete m_pRenderer;
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK RenderCore::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    std::vector<Camera*> &cameras = core->m_Cameras;

    size_t cameraCount = cameras.size();
    for (int i = 0; i < cameraCount; ++i)
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
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    IRenderer* renderer = core->m_pRenderer;
    ID3D11Device *d3dDevice = DXUTGetD3D11Device();
    ID3D11DeviceContext *d3dContext = DXUTGetD3D11DeviceContext();

    GfxDevice *device = new GfxDevice(d3dDevice);
    core->m_pDevice = device;
    core->m_pDeviceContext = new GfxDeviceContext(d3dContext);

    renderer->Init(core, device);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RenderCore::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11ReleasingSwapChain(void* pUserContext)
{

}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11DestroyDevice(void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    IRenderer* renderer = core->m_pRenderer;
    renderer->Destroy(core);
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    std::vector<Camera*> &cameras = core->m_Cameras;

    size_t cameraCount = cameras.size();
    for (int i = 0; i < cameraCount; ++i)
    {
        cameras[i]->FrameMove(fElapsedTime);
    }
    
    IRenderer* renderer = core->m_pRenderer;
    renderer->OnUpdate(core, fTime, fElapsedTime, nullptr);
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
    float fElapsedTime, void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    IRenderer* renderer = core->m_pRenderer;
    renderer->OnFrameRender(core, core->m_pDevice, core->m_pDeviceContext, fTime, fElapsedTime, nullptr);
}

void RenderCore::InternalInit(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight)
{
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

    // Require D3D_FEATURE_LEVEL_11_0
    DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, windowWidth, windowHeight);
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
