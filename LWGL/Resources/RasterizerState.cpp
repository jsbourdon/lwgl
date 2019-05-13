#include "RasterizerState.h"

using namespace lwgl;
using namespace resources;

RasterizerState::RasterizerState()
    : base()
    , m_pD3DRasterizerState(nullptr)
{

}

RasterizerState::~RasterizerState()
{
    SAFE_RELEASE(m_pD3DRasterizerState);
}