#include "Mesh.h"
#include "../Core/GfxDeviceContext.h"

using namespace lwgl;
using namespace resources;

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	m_DXUTMesh.Destroy();
}