#include "RenderCore.h"
#include "GfxDevice.h"
#include "GfxDeviceContext.h"

using namespace lwgl;
using namespace core;

RenderCore::RenderCore(IRenderer *renderer)
    : m_Renderer(renderer)
    , m_Device(nullptr)
    , m_DeviceContext(nullptr)
{
}

RenderCore::~RenderCore()
{
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK RenderCore::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
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
    IRenderer* renderer = core->m_Renderer;
    ID3D11Device *d3dDevice = DXUTGetD3D11Device();
    ID3D11DeviceContext *d3dContext = DXUTGetD3D11DeviceContext();

    GfxDevice *device = new GfxDevice(d3dDevice);
    core->m_Device = device;
    core->m_DeviceContext = new GfxDeviceContext(d3dContext);

    renderer->Init(device);

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

}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    IRenderer* renderer = core->m_Renderer;
    renderer->OnUpdate(fTime, fElapsedTime, nullptr);
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK RenderCore::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
    float fElapsedTime, void* pUserContext)
{
    RenderCore* core = static_cast<RenderCore*>(pUserContext);
    IRenderer* renderer = core->m_Renderer;
    renderer->OnFrameRender(core->m_Device, core->m_DeviceContext, fTime, fElapsedTime, nullptr);
}

void RenderCore::Init(wchar_t const *windowTitle, uint32_t windowWidth, uint32_t windowHeight)
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
