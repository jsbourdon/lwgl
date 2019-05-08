#pragma once

namespace lwgl
{
	namespace core
	{
		class GfxDevice;
		class GfxDeviceContext;

		class IRenderer
		{
		public:
			virtual bool Init(GfxDevice* device) = 0;
			virtual void Destroy(GfxDevice* device, GfxDeviceContext* context) = 0;
			virtual void OnUpdate(double fTime, float fElapsedTime, void* pUserContext) = 0;
			virtual void OnFrameRender(GfxDevice* device, GfxDeviceContext* context, double fTime, float fElapsedTime, void* pUserContext) = 0;
		};
	}
}