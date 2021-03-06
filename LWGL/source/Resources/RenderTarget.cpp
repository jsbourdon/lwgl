#include <pch.h>
#include "Resources/RenderTarget.h"
#include "Resources/Texture.h"

using namespace lwgl;
using namespace resources;

RenderTarget::RenderTarget()
    : base()
    , m_pRenderTexture(nullptr)
    , m_pRenderTargetView(nullptr)
{

}

RenderTarget::~RenderTarget()
{
    SAFE_RELEASE(m_pRenderTexture);
    SAFE_RELEASE(m_pRenderTargetView);
}