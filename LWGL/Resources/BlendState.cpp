#include "BlendState.h"

using namespace lwgl;
using namespace resources;

BlendState::BlendState()
    : base()
    , m_pBlendState(nullptr)
{

}

BlendState::~BlendState()
{
    SAFE_RELEASE(m_pBlendState);
}