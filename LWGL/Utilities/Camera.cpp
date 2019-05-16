#include <pch.h>
#include "Camera.h"

using namespace lwgl;
using namespace utilities;

Camera::Camera()
    : base()
{

}

Camera::~Camera() 
{

}

void Camera::Init(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_DXUTCamera.SetViewParams(worldPosition, lookAtWorldPosition);
    m_DXUTCamera.SetProjParams(fov, aspectRatio, nearPlane, farPlane);
    m_DXUTCamera.SetRotateButtons(false, false, false, true);
    m_DXUTCamera.SetScalers(0.01f, 50.0f);
}

Matrix4x4 Camera::GetViewMatrix() const 
{ 
    return m_DXUTCamera.GetViewMatrix();
}

Matrix4x4 Camera::GetProjMatrix() const 
{ 
    return m_DXUTCamera.GetProjMatrix();
}

void Camera::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_DXUTCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
}

void Camera::FrameMove(float fElapsedTime)
{
    m_DXUTCamera.FrameMove(fElapsedTime);
}
