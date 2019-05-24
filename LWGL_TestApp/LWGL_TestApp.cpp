// LWGL_TestApp.cpp : Defines the entry point for the application.
//

#include "LWGL_TestApp.h"
#include "Core/RenderCore.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Buffer.h"
#include "Resources/SamplerState.h"
#include "Pipeline/GfxPipeline.h"
#include "Descriptors/PipelineDescriptor.h"
#include "Descriptors/BufferDescriptor.h"
#include "Descriptors/SamplerStateDescriptor.h"
#include "Descriptors/ClearDescriptor.h"
#include "Utilities/Camera.h"
#include "Inputs/IInputReceiver.h"

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
    Vector3 positionWS;
    Vector3 directionWS;
    float penumbraCosTheta;
    float umbraCosTheta;
    float radius;
};

class BaseRenderer : public IRenderer, public IInputReceiver
{
private:

    static constexpr size_t MaxPointLightCount = 1000;
    static constexpr size_t MaxSpotLightCount = 1000;

public:

    bool Init(RenderCore *pRenderCore, GfxDevice *pDevice, GfxDeviceContext* pContext) override
    {
        pRenderCore->RegisterInputReceiver(this);

        m_pMesh = pDevice->CreateMesh(L"sponza\\sponza.sdkmesh");

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
        pipelineDesc.VertexShader.FilePath = L"BasicHLSL11_VS.hlsl";
        pipelineDesc.VertexShader.EntryPoint = "VSMain";
        pipelineDesc.VertexShader.DebugName = "BasicHLSL11_VS";

        pipelineDesc.FragmentShader.Type = ShaderType::FragmentShader;
        pipelineDesc.FragmentShader.FilePath = L"BasicHLSL11_PS.hlsl";
        pipelineDesc.FragmentShader.EntryPoint = "PSMain";
        pipelineDesc.FragmentShader.DebugName = "BasicHLSL11_PS";

        pipelineDesc.RasterizerState.CullMode = CullMode::Back;
        pipelineDesc.RasterizerState.Winding = Winding::FrontClockwise;

        m_pPipeline = pDevice->CreatePipeline(pipelineDesc);

        BufferDescriptor bufferDesc;
        bufferDesc.ByteSize = 128;
        bufferDesc.DebugName = "VS_ConstantBuffer";
        bufferDesc.StructureStride = 0;
        bufferDesc.Type = BufferType::Constants;
        bufferDesc.Usage = BufferUsage::GPU_ReadOnly_CPU_WriteOnly;

        m_pVSConstantBuffer = pDevice->CreateBuffer(bufferDesc);

        bufferDesc.ByteSize = 16;
        bufferDesc.DebugName = "PS_ConstantBuffer";
        bufferDesc.StructureStride = 0;
        bufferDesc.Type = BufferType::Constants;
        bufferDesc.Usage = BufferUsage::GPU_ReadOnly_CPU_WriteOnly;

        m_pPSConstantBuffer = pDevice->CreateBuffer(bufferDesc);

        bufferDesc.ByteSize = sizeof(PointLight) * MaxPointLightCount;
        bufferDesc.DebugName = "PointLights_Buffer";
        bufferDesc.StructureStride = sizeof(PointLight);
        bufferDesc.Type = BufferType::Structured;
        bufferDesc.Usage = BufferUsage::GPU_ReadOnly_CPU_WriteOnly;

        m_pPointLightsBuffer = pDevice->CreateBuffer(bufferDesc);

        bufferDesc.ByteSize = sizeof(SpotLight) * MaxSpotLightCount;
        bufferDesc.DebugName = "SpotLights_Buffer";
        bufferDesc.StructureStride = sizeof(SpotLight);
        bufferDesc.Type = BufferType::Structured;
        bufferDesc.Usage = BufferUsage::GPU_ReadOnly_CPU_WriteOnly;

        m_pSpotLightsBuffer = pDevice->CreateBuffer(bufferDesc);

        SamplerStateDescriptor samplerDesc;
        m_pSamplerState = pDevice->CreateSamplerState(samplerDesc);

        Vector4 cameraPositionWS = { 0.0f, 0.0f, -100.0f, 0.0f };
        Vector4 lookAtPositionWS = { 0.0f, 0.0f, 0.0f, 0.0f };
        
        m_pCamera = pRenderCore->CreateCamera(cameraPositionWS, lookAtPositionWS, lwgl::core::PI_ON_FOUR, 16.0f / 9.0f, 0.1f, 5000.0f);

        m_ClearDesc.ClearColor = true;
        m_ClearDesc.ColorClearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_ClearDesc.ClearDepth = true;
        m_ClearDesc.DepthClearValue = 1.0f;

        return true;
    }

    void Destroy(RenderCore *pRenderCore) override
    {
        SAFE_RELEASE(m_pMesh);
        SAFE_RELEASE(m_pPipeline);
        SAFE_RELEASE(m_pVSConstantBuffer);
        SAFE_RELEASE(m_pPSConstantBuffer);
        SAFE_RELEASE(m_pPointLightsBuffer);
        SAFE_RELEASE(m_pSpotLightsBuffer);
        SAFE_RELEASE(m_pCamera);
        SAFE_RELEASE(m_pSamplerState);
    }

    void OnUpdate(RenderCore *pRenderCore, GfxDeviceContext* pContext, double fTime, float fElapsedTime, void* pUserContext) override
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
        }
    }

    void OnFrameRender(RenderCore *pRenderCore, GfxDevice* pDevice, GfxDeviceContext* pContext, double fTime, float fElapsedTime, void* pUserContext) override
    {
        pContext->Clear(m_ClearDesc);

        Matrix4x4 *pConstantBufferData = static_cast<Matrix4x4*>(pContext->MapBuffer(m_pVSConstantBuffer, MapType::WriteDiscard));
        *pConstantBufferData = m_pCamera->GetViewMatrix() * m_pCamera->GetProjMatrix();
        *(pConstantBufferData + 1) = s_IdentityMatrix;
        pContext->UnmapBuffer(m_pVSConstantBuffer);

        pContext->SetupPipeline(m_pPipeline);
        pContext->BindBuffer(m_pVSConstantBuffer, Stage::VS, 0);
        pContext->BindBuffer(m_pPSConstantBuffer, Stage::PS, 0);
        pContext->BindBuffer(m_pPointLightsBuffer, Stage::PS, 1);
        pContext->BindBuffer(m_pSpotLightsBuffer, Stage::PS, 2);
        pContext->BindSampler(m_pSamplerState, Stage::PS, 0);
        pContext->DrawMesh(m_pMesh);
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
            SpotLight &light = m_SpotLights[m_NextSpotLightIndex++];
            light.positionWS = m_pCamera->GetWorldPosition();
            light.directionWS = m_pCamera->GetLookAtDirection();
            light.radius = 5000.0f;
            //light.penumbraCosTheta = 0.8660f;   // cos 30 degrees
            //light.umbraCosTheta = 0.7071f;      // cos 45 degrees
            light.penumbraCosTheta = 0.9397f;   // cos 20 degrees
            light.umbraCosTheta = 0.8660f;      // cos 30 degrees

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

    static const Matrix4x4 s_IdentityMatrix;

    Mesh*           m_pMesh = nullptr;
    GfxPipeline*    m_pPipeline = nullptr;
    Buffer*         m_pVSConstantBuffer = nullptr;
    Buffer*         m_pPSConstantBuffer = nullptr;
    Buffer*         m_pPointLightsBuffer = nullptr;
    Buffer*         m_pSpotLightsBuffer = nullptr;
    SamplerState*   m_pSamplerState = nullptr;
    Camera*         m_pCamera = nullptr;
    ClearDescriptor m_ClearDesc {};

    PointLight      m_PointLights[MaxPointLightCount];
    SpotLight       m_SpotLights[MaxSpotLightCount];
    uint32_t        m_NextPointLightIndex = 0;
    uint32_t        m_NextSpotLightIndex = 0;
    bool            m_PointLightAdded = false;
    bool            m_SpotLightAdded = false;
};

const Matrix4x4 BaseRenderer::s_IdentityMatrix =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
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

