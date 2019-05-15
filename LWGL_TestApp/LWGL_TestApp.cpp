// LWGL_TestApp.cpp : Defines the entry point for the application.
//

#include "LWGL_TestApp.h"
#include "Core/RenderCore.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Buffer.h"
#include "Pipeline/GfxPipeline.h"
#include "Descriptors/PipelineDescriptor.h"
#include "Descriptors/BufferDescriptor.h"
#include "Descriptors/SamplerStateDescriptor.h"
#include "Descriptors/ClearDescriptor.h"
#include "Utilities/Camera.h"

using namespace lwgl;
using namespace core;
using namespace resources;
using namespace descriptors;

class RendererTest : public IRenderer
{
public:

    ~RendererTest()
    {

    }

    bool Init(RenderCore *pRenderCore, GfxDevice *pDevice) override
    {
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

        SamplerStateDescriptor samplerDesc;
        m_pSamplerState = pDevice->CreateSamplerState(samplerDesc);

        Vector4 cameraPositionWS = { 0.0f, 0.0f, -100.0f, 0.0f };
        Vector4 lookAtPositionWS = { 0.0f, 0.0f, 0.0f, 0.0f };
        
        m_pCamera = pRenderCore->CreateCamera(cameraPositionWS, lookAtPositionWS, lwgl::core::PI_ON_FOUR, 16.0f / 9.0f, 0.1f, 1000.0f);

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
        SAFE_RELEASE(m_pCamera);
    }

    void OnUpdate(RenderCore *pRenderCore, double fTime, float fElapsedTime, void* pUserContext) override
    {
        
    }

    void OnFrameRender(RenderCore *pRenderCore, GfxDevice* pDevice, GfxDeviceContext* pContext, double fTime, float fElapsedTime, void* pUserContext) override
    {
        pContext->Clear(m_ClearDesc);

        Matrix4x4 viewMatrix = m_pCamera->GetViewMatrix();
        Matrix4x4 projMatrix = m_pCamera->GetProjMatrix();
        Matrix4x4 viewProjection = viewMatrix * projMatrix;

        Matrix4x4 *pConstantBufferData = static_cast<Matrix4x4*>(pContext->MapBuffer(m_pVSConstantBuffer, MapType::WriteDiscard));
        *pConstantBufferData = viewProjection; // m_pCamera->GetViewMatrix() * m_pCamera->GetProjMatrix();
        *(pConstantBufferData + 1) = m_pCamera->GetViewMatrix();
        pContext->UnmapBuffer(m_pVSConstantBuffer);

        pContext->SetupPipeline(m_pPipeline);
        pContext->BindBuffer(m_pVSConstantBuffer, Stage::VS, 0);
        pContext->BindSampler(m_pSamplerState, Stage::PS, 0);
        pContext->DrawMesh(m_pMesh);
    }

private:

    Mesh*           m_pMesh = nullptr;
    GfxPipeline*    m_pPipeline = nullptr;
    Buffer*         m_pVSConstantBuffer = nullptr;
    SamplerState*   m_pSamplerState = nullptr;
    Camera*         m_pCamera = nullptr;
    ClearDescriptor m_ClearDesc {};
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RenderCore renderCore;
    renderCore.Init<RendererTest>(L"Test", 1280, 720);
    renderCore.StartRenderLoop();

    return 0;
}

