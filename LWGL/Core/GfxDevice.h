#pragma once

#include <stdafx.h>
#include "../Resources/Mesh.h"

namespace lwgl
{
	using namespace resources;

	namespace core
	{
		class GfxDevice
		{
		public:

			GfxDevice(ID3D11Device *d3dDevice);
			~GfxDevice();

			Mesh* CreateMesh(const wchar_t* filePath);

		private:

			ID3D11Device* m_D3DDevice;
		};
	}
}