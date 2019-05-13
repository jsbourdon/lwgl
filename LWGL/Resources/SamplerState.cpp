#include "SamplerState.h"

using namespace lwgl;
using namespace resources;

SamplerState::SamplerState()
    : base()
    , m_pSamplerState(nullptr)
{

}

SamplerState::~SamplerState()
{
    SAFE_RELEASE(m_pSamplerState);
}
