#pragma once

#include <stdafx.h>

namespace lwgl
{
	namespace resources
	{
		class Mesh;
	}

	using namespace resources;

	namespace core
	{
		class GfxDeviceContext
		{
		public:

			GfxDeviceContext(ID3D11DeviceContext* d3dContext);
			~GfxDeviceContext();

			void DrawMesh(Mesh *mesh);

		private:

			ID3D11DeviceContext* m_D3DContext;
		};
	}
}