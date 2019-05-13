// LWGL_TestApp.cpp : Defines the entry point for the application.
//

#include "LWGL_TestApp.h"
#include "Core/RenderCore.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Pipeline/GfxPipeline.h"
#include "Descriptors/PipelineDescriptor.h"

using namespace lwgl;
using namespace core;
using namespace resources;
using namespace descriptors;

class RendererTest : public IRenderer
{
public:

    bool Init(GfxDevice* device) override
    {
        m_Mesh = device->CreateMesh(L"sponza/sponza.sdkmesh");

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

        m_Pipeline = device->CreatePipeline(pipelineDesc);

        return true;
    }

    void Destroy(GfxDevice* device, GfxDeviceContext* context) override
    {
        SAFE_RELEASE(m_Mesh);
        SAFE_RELEASE(m_Pipeline);
    }

    void OnUpdate(double fTime, float fElapsedTime, void* pUserContext) override
    {

    }

    void OnFrameRender(GfxDevice* device, GfxDeviceContext* context, double fTime, float fElapsedTime, void* pUserContext) override
    {
        context->SetupPipeline(m_Pipeline);
        context->DrawMesh(m_Mesh);
    }

private:

    Mesh* m_Mesh = nullptr;
    GfxPipeline* m_Pipeline = nullptr;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RendererTest renderer;
    RenderCore renderCore(&renderer);
    renderCore.Init(L"Test", 1280, 720);
    renderCore.StartRenderLoop();

    return 0;
}

