#include "GfxDevice.h"
#include <stdafx.h>

using namespace lwgl;
using namespace core;

GfxDevice::GfxDevice(ID3D11Device* d3dDevice)
	: m_D3DDevice(d3dDevice)
{

}

GfxDevice::~GfxDevice()
{

}

Mesh* GfxDevice::CreateMesh(const wchar_t* filePath)
{
	Mesh* mesh = new Mesh();
	mesh->m_DXUTMesh.Create(m_D3DDevice, filePath);
	return mesh;
}
