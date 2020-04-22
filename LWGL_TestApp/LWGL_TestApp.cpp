// LWGL_TestApp.cpp : Defines the entry point for the application.
//

#include "LWGL_TestApp.h"
#include "Core/RenderCore.h"
#include "Core/IRenderer.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Core/Globals.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Buffer.h"
#include "Resources/Texture.h"
#include "Resources/TextureArray.h"
#include "Resources/SamplerState.h"
#include "Pipeline/GfxPipeline.h"
#include "Descriptors/PipelineDescriptor.h"
#include "Descriptors/BufferDescriptor.h"
#include "Descriptors/SamplerStateDescriptor.h"
#include "Descriptors/ClearDescriptor.h"
#include "Descriptors/TextureDescriptor.h"
#include "Utilities/Camera.h"
#include "Utilities/FixedCamera.h"
#include "Inputs/IInputReceiver.h"
#include "Debugging/DebuggingFeatures.h"

using namespace lwgl;
using namespace core;
using namespace resources;
using namespace descriptors;

struct PointLight
{
    Vector3 positionWS;
    float   radius;
};

struct SpotLight
{
    Matrix4x4 viewProjection; // aligned on 16 bytes
    Vector3 positionWS;
    Vector3 directionWS;
    float penumbraCosTheta;
    float umbraCosTheta;
    float radius;
};

class BaseRenderer : public IRenderer, public IInputReceiver
{
private:

    static constexpr size_t     MaxPointLightCount = 1000;
    static constexpr size_t     MaxSpotLightCount = 10;
    static constexpr uint32_t   ShadowMapSize = 1024;

    static constexpr float      SpotLightAngleRad = 0.5236f; // 30 degrees
    static constexpr float      SpotLightUmbraAngleCos = 0.8660f; // cos(SpotLightAngleRad)
    static constexpr float      SpotLightPenumbraAngleCos = 0.9397f; // cos 20 degrees
    static constexpr float      SpotLightFarPlane = 2000.0f;

public:

    bool Init(RenderCore *pRenderCore, GfxDevice *pDevice, GfxDeviceContext* pContext) override
    {
        pRenderCore->RegisterInputReceiver(this);

        m_pMesh = pDevice->CreateMesh(L"sponza\\sponza.sdkmesh");

        // Forward pipeline
        {
            PipelineDescriptor pipelineDesc = {};

            pipelineDesc.BlendState.IsEnabled = false;

            pipelineDesc.DepthStencilState.IsDepthTestEnabled = true;
            pipelineDesc.DepthStencilState.IsDepthWriteEnabled = true;
            pipelineDesc.DepthStencilState.DepthFunction = ComparisonFunction::LESS_EQUAL;
            pipelineDesc.DepthStencilState.IsStencilEnabled = false;

            pipelineDesc.InputLayout.Elements.push_back({ InputLayoutSemantic::Position, 0 });
            pipelineDesc.InputLayout.Elements.push_back({ InputLayoutSemantic::Normal, 0 });
            pipelineDesc.InputLayout.Elements.push_back({ InputLayoutSemantic::UV, 0 });
            pipelineDesc.InputLayout.Elements.push_back({ InputLayoutSemantic::Tangent, 0 });

            pipelineDesc.VertexShader.Type = ShaderType::VertexShader;
            //pipelineDesc.VertexShader.FilePath = L"BasicHLSL11_VS.hlsl";
            pipelineDesc.VertexShader.BytecodeFilePath = L"BasicHLSL11_VS.cso";
            pipelineDesc.VertexShader.EntryPoint = "VSMain";
            pipelineDesc.VertexShader.DebugName = "BasicHLSL11_VS";

            pipelineDesc.FragmentShader.Type = ShaderType::FragmentShader;
            //pipelineDesc.FragmentShader.FilePath = L"BasicHLSL11_PS.hlsl";
            pipelineDesc.FragmentShader.BytecodeFilePath = L"BasicHLSL11_PS.cso";
            pipelineDesc.FragmentShader.EntryPoint = "PSMain";
            pipelineDesc.FragmentShader.DebugName = "BasicHLSL11_PS";

            pipelineDesc.RasterizerState.CullMode = CullMode::Back;
            pipelineDesc.RasterizerState.Winding = Winding::FrontClockwise;

            m_pPipeline = pDevice->CreatePipeline(pipelineDesc);
        }

        // Shadow maps pipeline
        {
            PipelineDescriptor pipelineDesc = {};

            pipelineDesc.BlendState.IsEnabled = false;
            pipelineDesc.BlendState.ColorWrite = false;

            pipelineDesc.DepthStencilState.IsDepthTestEnabled = true;
            pipelineDesc.DepthStencilState.IsDepthWriteEnabled = true;
            pipelineDesc.DepthStencilState.DepthFunction = ComparisonFunction::LESS_EQUAL;
            pipelineDesc.DepthStencilState.IsStencilEnabled = false;

            pipelineDesc.InputLayout.Elements.push_back({ InputLayoutSemantic::Position, 0 });

            pipelineDesc.VertexShader.Type = ShaderType::VertexShader;
            //pipelineDesc.VertexShader.FilePath = L"ShadowMap_VS.hlsl";
            pipelineDesc.VertexShader.BytecodeFilePath = L"ShadowMap_VS.cso";
            pipelineDesc.VertexShader.EntryPoint = "VSMain";
            pipelineDesc.VertexShader.DebugName = "ShadowMap_VS";

            pipelineDesc.FragmentShader.Type = ShaderType::FragmentShader;
            pipelineDesc.FragmentShader.FilePath = nullptr;

            pipelineDesc.RasterizerState.CullMode = CullMode::Back;
            pipelineDesc.RasterizerState.Winding = Winding::FrontClockwise;

            m_pShadowMapPipeline = pDevice->CreatePipeline(pipelineDesc);
        }

        // Full screen test pipeline
        {
            PipelineDescriptor pipelineDesc = {};

            pipelineDesc.BlendState.IsEnabled = false;

            pipelineDesc.DepthStencilState.IsDepthTestEnabled = false;
            pipelineDesc.DepthStencilState.IsDepthWriteEnabled = false;
            pipelineDesc.DepthStencilState.IsStencilEnabled = false;

            pipelineDesc.VertexShader.Type = ShaderType::VertexShader;
            pipelineDesc.VertexShader.FilePath = nullptr;

            pipelineDesc.FragmentShader.Type = ShaderType::FragmentShader;
            //pipelineDesc.FragmentShader.FilePath = L"Test.hlsl";
            pipelineDesc.FragmentShader.BytecodeFilePath = L"Test.cso";
            pipelineDesc.FragmentShader.EntryPoint = "PSMain";
            pipelineDesc.FragmentShader.DebugName = "Test";

            pipelineDesc.RasterizerState.CullMode = CullMode::Back;
            pipelineDesc.RasterizerState.Winding = Winding::FrontClockwise;

            m_pFullScreenPipeline = pDevice->CreatePipeline(pipelineDesc);
        }

        BufferDescriptor bufferDesc;

        // Forward VS constant buffer
        {
            bufferDesc.ByteSize = 128;
            bufferDesc.DebugName = "VS_ConstantBuffer";
            bufferDesc.StructureStride = 0;
            bufferDesc.Type = BufferType::Constants;
            bufferDesc.Usage = ResourceUsage::GPU_ReadOnly_CPU_WriteOnly;

            m_pVSConstantBuffer = pDevice->CreateBuffer(bufferDesc);
        }

        // Shadow map VS constant buffer
        {
            bufferDesc.ByteSize = 128;
            bufferDesc.DebugName = "VS_ShadowMapCB";
            bufferDesc.StructureStride = 0;
            bufferDesc.Type = BufferType::Constants;
            bufferDesc.Usage = ResourceUsage::GPU_ReadOnly_CPU_WriteOnly;

            m_pVSShadowMapConstantBuffer = pDevice->CreateBuffer(bufferDesc);
        }

        // Forward PS constant buffer
        {
            bufferDesc.ByteSize = 16;
            bufferDesc.DebugName = "PS_ConstantBuffer";
            bufferDesc.StructureStride = 0;
            bufferDesc.Type = BufferType::Constants;
            bufferDesc.Usage = ResourceUsage::GPU_ReadOnly_CPU_WriteOnly;

            m_pPSConstantBuffer = pDevice->CreateBuffer(bufferDesc);
        }

        // Point lights structured buffer
        {
            bufferDesc.ByteSize = sizeof(PointLight) * MaxPointLightCount;
            bufferDesc.DebugName = "PointLights_Buffer";
            bufferDesc.StructureStride = sizeof(PointLight);
            bufferDesc.Type = BufferType::Structured;
            bufferDesc.Usage = ResourceUsage::GPU_ReadOnly_CPU_WriteOnly;

            m_pPointLightsBuffer = pDevice->CreateBuffer(bufferDesc);
        }

        // Spot lights structured buffer
        {
            bufferDesc.ByteSize = sizeof(SpotLight) * MaxSpotLightCount;
            bufferDesc.DebugName = "SpotLights_Buffer";
            bufferDesc.StructureStride = sizeof(SpotLight);
            bufferDesc.Type = BufferType::Structured;
            bufferDesc.Usage = ResourceUsage::GPU_ReadOnly_CPU_WriteOnly;

            m_pSpotLightsBuffer = pDevice->CreateBuffer(bufferDesc);
        }

        // Spot light shadow maps
        {
            TextureDescriptor desc;
            desc.Type = TextureType::DepthStencil;
            desc.BindFlags = TextureBindFlags::ShaderResource;
            desc.Format = PixelFormat::R24_UNORM_X8_TYPELESS;
            desc.Usage = ResourceUsage::GPU_ReadWrite;
            desc.Width = ShadowMapSize;
            desc.Height = ShadowMapSize;
            desc.ArraySize = MaxSpotLightCount;
            desc.MipLevels = 1;
            desc.SampleCount = 1;
            desc.DebugName = "ShadowMaps";

            m_pSpotLightShadowMaps = pDevice->CreateTextureArray(desc);
        }

        SamplerStateDescriptor samplerDesc;
        m_pSamplerState = pDevice->CreateSamplerState(samplerDesc);

        Vector4 cameraPositionWS = { 0.0f, 0.0f, -100.0f, 0.0f };
        Vector4 lookAtPositionWS = { 0.0f, 0.0f, 0.0f, 0.0f };
        
        m_pCamera = pRenderCore->CreateCamera(cameraPositionWS, lookAtPositionWS, lwgl::core::PI_ON_FOUR, 16.0f / 9.0f, 0.1f, 5000.0f);
        
        m_ClearDesc.ClearColor = true;
        m_ClearDesc.ColorClearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_ClearDesc.ClearDepth = true;
        m_ClearDesc.DepthClearValue = 1.0f;

        m_ShadowMapClearDesc.ClearColor = false;
        m_ShadowMapClearDesc.ClearDepth = true;
        m_ShadowMapClearDesc.DepthClearValue = 1.0f;

        DebuggingFeatures *pDebugFeatures = pRenderCore->GetDebuggingFeatures();
        pDebugFeatures->ShowFPS(true);
        pDebugFeatures->ShowMainCameraPosition(true);

        return true;
    }

    void Destroy(RenderCore *pRenderCore) override
    {
        SAFE_RELEASE(m_pMesh);
        SAFE_RELEASE(m_pPipeline);
        SAFE_RELEASE(m_pShadowMapPipeline);
        SAFE_RELEASE(m_pFullScreenPipeline);
        SAFE_RELEASE(m_pVSConstantBuffer);
        SAFE_RELEASE(m_pVSShadowMapConstantBuffer);
        SAFE_RELEASE(m_pPSConstantBuffer);
        SAFE_RELEASE(m_pPointLightsBuffer);
        SAFE_RELEASE(m_pSpotLightsBuffer);
        SAFE_RELEASE(m_pCamera);
        SAFE_RELEASE(m_pSamplerState);
        SAFE_RELEASE(m_pSpotLightShadowMaps);
    }

    void OnUpdate(RenderCore *pRenderCore, GfxDeviceContext* pContext, double fTimeSec, float fElapsedTimeSec, void* pUserContext) override
    {
        if (m_PointLightAdded | m_SpotLightAdded)
        {
            uint32_t *pLightCount = static_cast<uint32_t*>(pContext->MapBuffer(m_pPSConstantBuffer, MapType::WriteDiscard));
            *pLightCount = m_NextPointLightIndex;
            *(pLightCount + 1) = m_NextSpotLightIndex;
            pContext->UnmapBuffer(m_pPSConstantBuffer);
        }

        if (m_PointLightAdded)
        {
            m_PointLightAdded = false;

            void *pPointLightBufferPtr = pContext->MapBuffer(m_pPointLightsBuffer, MapType::WriteDiscard);
            memcpy(pPointLightBufferPtr, m_PointLights, sizeof(m_PointLights));
            pContext->UnmapBuffer(m_pPointLightsBuffer);
        }

        if (m_SpotLightAdded)
        {
            m_SpotLightAdded = false;

            void *pSpotLightBufferPtr = pContext->MapBuffer(m_pSpotLightsBuffer, MapType::WriteDiscard);
            memcpy(pSpotLightBufferPtr, m_SpotLights, sizeof(m_SpotLights));
            pContext->UnmapBuffer(m_pSpotLightsBuffer);

            RenderNewShadowMap(pContext);
        }

        Matrix4x4 *pConstantBufferData = static_cast<Matrix4x4*>(pContext->MapBuffer(m_pVSConstantBuffer, MapType::WriteDiscard));
        *pConstantBufferData = m_pCamera->GetViewMatrix() * m_pCamera->GetProjMatrix();;
        *(pConstantBufferData + 1) = s_IdentityMatrix;
        pContext->UnmapBuffer(m_pVSConstantBuffer);
    }

    void RenderNewShadowMap(GfxDeviceContext* pContext)
    {
        const uint32_t lastShadowMapIndex = m_NextSpotLightIndex - 1;
        const SpotLight &spotLight = m_SpotLights[lastShadowMapIndex];
        FixedCamera &shadowCamera = m_SpotShadowMapCameras[lastShadowMapIndex];

        Matrix4x4 *pShadowMapCBData = static_cast<Matrix4x4*>(pContext->MapBuffer(m_pVSShadowMapConstantBuffer, MapType::WriteDiscard));
        *pShadowMapCBData = shadowCamera.GetViewMatrix();
        *(pShadowMapCBData + 1) = shadowCamera.GetProjMatrix();
        pContext->UnmapBuffer(m_pVSShadowMapConstantBuffer);

        pContext->UnbindRange(Stage::PS, 0, 4);
        pContext->BindRenderTargets(nullptr, 0, m_pSpotLightShadowMaps, lastShadowMapIndex);
        pContext->SetupPipeline(m_pShadowMapPipeline);
        pContext->BindBuffer(m_pVSShadowMapConstantBuffer, Stage::VS, 0);
        pContext->Clear(m_ShadowMapClearDesc);
        pContext->DrawMesh(m_pMesh);

        pContext->BindSwapChain();
    }

    void OnFrameRender(RenderCore *pRenderCore, GfxDevice* pDevice, GfxDeviceContext* pContext, double fTimeSec, float fElapsedTimeSec, void* pUserContext) override
    {
        pContext->Clear(m_ClearDesc);

        pContext->SetupPipeline(m_pPipeline);
        pContext->BindBuffer(m_pVSConstantBuffer, Stage::VS, 0);
        pContext->BindBuffer(m_pPSConstantBuffer, Stage::PS, 0);
        pContext->BindBuffer(m_pPointLightsBuffer, Stage::PS, 1);
        pContext->BindBuffer(m_pSpotLightsBuffer, Stage::PS, 2);
        pContext->BindTexture(m_pSpotLightShadowMaps, Stage::PS, 3);
        pContext->BindSampler(m_pSamplerState, Stage::PS, 0);
        pContext->DrawMesh(m_pMesh);

//         pContext->SetupPipeline(m_pFullScreenPipeline);
//         pContext->DrawFullScreenTriangle();
    }

    void AddPointLight()
    {
        if (m_NextPointLightIndex < MaxPointLightCount)
        {
            PointLight &light = m_PointLights[m_NextPointLightIndex++];
            light.positionWS = m_pCamera->GetWorldPosition();
            light.radius = 500.0f;

            m_PointLightAdded = true;
        }
    }

    void AddSpotLight()
    {
        if (m_NextSpotLightIndex < MaxSpotLightCount)
        {
            const uint32_t currentSpotLightIndex = m_NextSpotLightIndex++;

            Vector3 camLookAtPoint = m_pCamera->GetLookAtPoint();
            FixedCamera &shadowCamera = m_SpotShadowMapCameras[currentSpotLightIndex];
            shadowCamera.Init(lwgl::math::ConvertVector(m_pCamera->GetWorldPosition()), lwgl::math::ConvertVector(camLookAtPoint), SpotLightAngleRad * 2, 1.0f, 0.1f, SpotLightFarPlane);

            SpotLight &light = m_SpotLights[currentSpotLightIndex];
            light.viewProjection = shadowCamera.GetViewMatrix() * shadowCamera.GetProjMatrix();
            light.positionWS = m_pCamera->GetWorldPosition();
            light.directionWS = m_pCamera->GetLookAtDirection();
            light.radius = SpotLightFarPlane;
            light.penumbraCosTheta = SpotLightPenumbraAngleCos;
            light.umbraCosTheta = SpotLightUmbraAngleCos;

            m_SpotLightAdded = true;
        }
    }

    void OnKeyDown(uint32_t keyCode, bool firstDown) override
    {
        if (firstDown)
        {
            switch (keyCode)
            {
            case 0x4F:
                AddPointLight();
                break;
            case 0x50:
                AddSpotLight();
                break;
            }
        }
    }

    void OnKeyUp(uint32_t keyCode) override {}

private:

    Mesh*               m_pMesh { nullptr };
    GfxPipeline*        m_pPipeline { nullptr };
    GfxPipeline*        m_pShadowMapPipeline { nullptr };
    GfxPipeline*        m_pFullScreenPipeline { nullptr };
    Buffer*             m_pVSConstantBuffer { nullptr };
    Buffer*             m_pVSShadowMapConstantBuffer { nullptr };
    Buffer*             m_pPSConstantBuffer { nullptr };
    Buffer*             m_pPointLightsBuffer { nullptr };
    Buffer*             m_pSpotLightsBuffer { nullptr };
    SamplerState*       m_pSamplerState { nullptr };
    Camera*             m_pCamera { nullptr };
    ClearDescriptor     m_ClearDesc {};
    ClearDescriptor     m_ShadowMapClearDesc {};

    PointLight          m_PointLights[MaxPointLightCount] {};
    SpotLight           m_SpotLights[MaxSpotLightCount] {};
    FixedCamera         m_SpotShadowMapCameras[MaxSpotLightCount] {};
    TextureArray*       m_pSpotLightShadowMaps { nullptr };
    uint32_t            m_NextPointLightIndex { 0 };
    uint32_t            m_NextSpotLightIndex { 0 };
    bool                m_PointLightAdded { false };
    bool                m_SpotLightAdded { false };
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RenderCore *pRenderCore = RenderCore::CreateCore();
    pRenderCore->Init<BaseRenderer>(L"Test", 1280, 720);
    pRenderCore->StartRenderLoop();
    pRenderCore->Release();

    return DXUTGetExitCode();
}

