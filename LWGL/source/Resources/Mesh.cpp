#include <pch.h>
#include "Resources/Mesh.h"
#include "Core/GfxDeviceContext.h"

using namespace lwgl;
using namespace resources;

Mesh::~Mesh()
{
    m_DXUTMesh.Destroy();
}