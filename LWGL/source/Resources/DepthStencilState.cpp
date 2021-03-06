#include <pch.h>
#include "Resources/DepthStencilState.h"

using namespace lwgl;
using namespace resources;

DepthStencilState::DepthStencilState()
    : base()
    , m_pDepthStencilState(nullptr)
{

}
DepthStencilState::~DepthStencilState()
{
    SAFE_RELEASE(m_pDepthStencilState);
}