#include "GfxDeviceContext.h"
#include "../Resources/Mesh.h"

using namespace lwgl;
using namespace core;
using namespace resources;

GfxDeviceContext::GfxDeviceContext(ID3D11DeviceContext* d3dContext)
	: m_D3DContext(d3dContext)
{

}

GfxDeviceContext::~GfxDeviceContext()
{

}

void GfxDeviceContext::DrawMesh(Mesh *mesh)
{
	mesh->m_DXUTMesh.Render(m_D3DContext);
}
