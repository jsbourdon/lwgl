#include <pch.h>
#include "Mesh.h"
#include "../Core/GfxDeviceContext.h"

using namespace lwgl;
using namespace resources;

Mesh::~Mesh()
{
    m_DXUTMesh.Destroy();
}