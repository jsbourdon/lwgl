#include <pch.h>
#include "DebuggingFeatures.h"
#include "DebugResourceManager.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"

using namespace lwgl;
using namespace debugging;

DebuggingFeatures::DebuggingFeatures()
#ifdef _DEBUG
    : m_pRscManager(new DebugResourceManager())
    , m_pDevice(nullptr)
    , m_pContext(nullptr)
    , m_AverageFPS(0.0f)
    , m_AccumulatedFPSElapsedTimeSec(0.0f)
    , m_AccumulatedFPSFrames(0)
    , m_ShowFPS(false)
#endif
{

}

DebuggingFeatures::~DebuggingFeatures()
{
#ifdef _DEBUG
    SAFE_RELEASE(m_pRscManager);
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pContext);
#endif
}

bool DebuggingFeatures::Init(GfxDevice *pDevice, GfxDeviceContext *pContext)
{
#ifdef _DEBUG
    pDevice->AddRef();
    pContext->AddRef();

    m_pDevice = pDevice;
    m_pContext = pContext;

    return m_pRscManager->Init(pDevice->GetNativeDevice(), pContext->GetNativeContext());
#else
    return true;
#endif
}

bool DebuggingFeatures::OnSwapChainResized(GfxDevice *pDevice, const NativeSurfaceDescriptor *pSurfaceDesc)
{
#ifdef _DEBUG
    return m_pRscManager->OnSwapChainResized(pDevice->GetNativeDevice(), pSurfaceDesc);
#else
    return true;
#endif
}

void DebuggingFeatures::OnSwapChainReleased()
{
#ifdef _DEBUG
    m_pRscManager->OnSwapChainReleased();
#endif
}

void DebuggingFeatures::ShowFPS(bool show)
{
#ifdef _DEBUG
    m_ShowFPS = show;
#endif
}

void DebuggingFeatures::OnUpdate(double fTimeSec, float fElapsedTimeSec)
{
#ifdef _DEBUG
    if (m_ShowFPS)
    {
        UpdateFPS(fElapsedTimeSec);
    }
#endif
}

void DebuggingFeatures::OnFrameRender(double fTimeSec, float fElapsedTimeSec)
{
#ifdef _DEBUG
    if (m_ShowFPS)
    {
        RenderFPS();
    }
#endif
}

void DebuggingFeatures::UpdateFPS(float fElapsedTimeSec)
{
#ifdef _DEBUG
    m_AccumulatedFPSElapsedTimeSec += fElapsedTimeSec;
    ++m_AccumulatedFPSFrames;

    if (m_AccumulatedFPSElapsedTimeSec >= s_FPSUpdateRateSec)
    {
        m_AverageFPS = m_AccumulatedFPSFrames / m_AccumulatedFPSElapsedTimeSec;
        m_AccumulatedFPSElapsedTimeSec = 0.0f;
        m_AccumulatedFPSFrames = 0;
    }
#endif
}

void DebuggingFeatures::RenderFPS()
{
#ifdef _DEBUG
    static wchar_t s_FPSString[64];
    swprintf_s(s_FPSString, L"Average FPS: %.2f", m_AverageFPS);
    DrawText(s_FPSString, { 0, 0, 400, 50 }, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
}

void DebuggingFeatures::DrawText(const wchar_t *pText, const RECT &pos, const Vector4 &color)
{
    uint32_t backBufferWidth, backBufferHeight;
    m_pRscManager->GetBackBufferSize(backBufferWidth, backBufferHeight);

    m_pRscManager->SetupDrawTextResources(m_pContext->GetNativeContext());

    DrawText11DXUT(
        m_pDevice->GetNativeDevice(),
        m_pContext->GetNativeContext(),
        pText,
        pos,
        DirectX::XMFLOAT4(color),
        static_cast<float>(backBufferWidth),
        static_cast<float>(backBufferHeight),
        false);

    m_pRscManager->CleanDrawTextResources(m_pDevice->GetNativeDevice(), m_pContext->GetNativeContext());
}
