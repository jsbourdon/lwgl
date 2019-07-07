#include <pch.h>
#include "FixedCamera.h"

using namespace lwgl;
using namespace utilities;

void FixedCamera::Init(const Vector4 &worldPosition, const Vector4 &lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_WorldPosition = lwgl::math::ConvertVector(worldPosition);
    m_ViewMatrix = DirectX::XMMatrixLookAtLH(worldPosition, lookAtWorldPosition, { 0.0f, 1.0f, 0.0f });
    m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
    m_ViewSpaceZParams = lwgl::math::GetViewSpaceZParams(nearPlane, farPlane);
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
}
