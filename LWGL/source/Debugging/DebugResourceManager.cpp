#include <pch.h>
#include "Debugging/DebugResourceManager.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Descriptors/SurfaceDescriptor.h"

using namespace lwgl;
using namespace debugging;

DebugResourceManager::~DebugResourceManager()
{
    m_DXUTRscManager.OnD3D11DestroyDevice();
}

bool DebugResourceManager::Init(GfxNativeDevice *pDevice, GfxNativeDeviceContext *pContext)
{
    DXUTSetMediaSearchPath(L"..\\3rd\\DXUT\\Media\\");
    CHECK_HRESULT_RETURN_VALUE(m_DXUTRscManager.OnD3D11CreateDevice(pDevice, pContext), false);
    return true;
}

bool DebugResourceManager::OnSwapChainResized(GfxNativeDevice *pDevice, const NativeSurfaceDescriptor *pSurfaceDesc)
{
    CHECK_HRESULT_RETURN_VALUE(m_DXUTRscManager.OnD3D11ResizedSwapChain(pDevice, pSurfaceDesc), false);
    return true;
}

void DebugResourceManager::OnSwapChainReleased()
{
    m_DXUTRscManager.OnD3D11ReleasingSwapChain();
}

void DebugResourceManager::SetupDrawTextResources(GfxNativeDeviceContext *pContext)
{
    BeginText11();
    m_DXUTRscManager.ApplyRenderUI11(pContext);
}

void DebugResourceManager::CleanDrawTextResources(GfxNativeDevice *pDevice, GfxNativeDeviceContext *pContext)
{
    EndText11(pDevice, pContext);
}

void DebugResourceManager::GetBackBufferSize(uint32_t &width, uint32_t &height)
{
    width = m_DXUTRscManager.m_nBackBufferWidth;
    height = m_DXUTRscManager.m_nBackBufferHeight;
}
