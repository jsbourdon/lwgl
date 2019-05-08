// LWGL_TestApp.cpp : Defines the entry point for the application.
//

#include "LWGL_TestApp.h"
#include "Core/RenderCore.h"
#include "Core/GfxDevice.h"
#include "Core/GfxDeviceContext.h"
#include "Resources/Mesh.h"

using namespace lwgl;
using namespace core;
using namespace resources;

class RendererTest : public IRenderer
{
public:

	bool Init(GfxDevice* device) override
	{
		m_Mesh = device->CreateMesh(L"sponza/sponza.sdkmesh");
		return true;
	}

	void Destroy(GfxDevice* device, GfxDeviceContext* context) override
	{

	}

	void OnUpdate(double fTime, float fElapsedTime, void* pUserContext) override
	{

	}

	void OnFrameRender(GfxDevice* device, GfxDeviceContext* context, double fTime, float fElapsedTime, void* pUserContext) override
	{
		context->DrawMesh(m_Mesh);
	}

private:

	Mesh* m_Mesh = nullptr;
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
	renderCore.Init(L"Test", 1920, 1080);
	renderCore.StartRenderLoop();

	return 0;
}

