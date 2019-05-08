#pragma once

#include <stdafx.h>
#include "../3rd/DXUT/Optional/SDKmesh.h"
#include "RefCountedResource.h"

namespace lwgl
{
	namespace core
	{
		class GfxDevice;
		class GfxDeviceContext;
	}

	using namespace core;

	namespace resources
	{
		class Mesh : public RefCountedResource<Mesh>
		{
		private:

			friend class GfxDevice;
			friend class GfxDeviceContext;

			Mesh();
			~Mesh();

			CDXUTSDKMesh m_DXUTMesh;
		};
	}
}