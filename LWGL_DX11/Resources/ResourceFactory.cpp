#include <pch.h>
#include "ResourceFactory.h"

using namespace lwgl;
using namespace lwgl::resources;

GpuDeviceHandle CreateGfxDevice()
{
    UINT flags = 0;
#if defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ID3D11Device *pDevice = nullptr;

    D3D_FEATURE_LEVEL levels[] = 
    { 
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    CHECK_HRESULT_RETURN_VALUE(
        D3D11CreateDevice(
            nullptr, 
            D3D_DRIVER_TYPE_HARDWARE, 
            nullptr, 
            flags, 
            levels, 
            ARRAY_SIZE(levels), 
            D3D11_SDK_VERSION, 
            &pDevice, 
            nullptr, 
            nullptr),
        lwgl::Handle_NULL);

    return reinterpret_cast<GpuDeviceHandle>(pDevice);
}

void DestroyGfxDevice(GpuDeviceHandle hdl)
{
    ID3D11Device *pDevice = reinterpret_cast<ID3D11Device*>(hdl);
    pDevice->Release();
}

CommandQueueHandle CreateCommandQueue(GpuDeviceHandle device, CommandQueueType type)
{
    ID3D11DeviceContext *pContext;
    ID3D11Device *pDevice = reinterpret_cast<ID3D11Device*>(device);
    pDevice->GetImmediateContext(&pContext);

    return reinterpret_cast<CommandQueueHandle>(pContext);
}

void DestroyCommandQueue(CommandQueueHandle hdl)
{
    ID3D11DeviceContext *pContext = reinterpret_cast<ID3D11DeviceContext*>(hdl);
    pContext->Release();
}
