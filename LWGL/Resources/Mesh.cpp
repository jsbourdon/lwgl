#include "Mesh.h"
#include "../Core/GfxDeviceContext.h"

using namespace lwgl;
using namespace resources;

Mesh::Mesh()
    : base()
{

}

Mesh::~Mesh()
{
    m_DXUTMesh.Destroy();
}