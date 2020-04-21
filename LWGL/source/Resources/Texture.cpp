#include <pch.h>
#include "Resources/Texture.h"

using namespace lwgl;
using namespace resources;

Texture::~Texture()
{
    SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pSRV);
    SAFE_RELEASE(m_pRTV);
}
