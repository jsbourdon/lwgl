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
    CURSORINFO cinfo = {};
    cinfo.cbSize = sizeof(CURSORINFO);
    if (GetCursorInfo(&cinfo) && cinfo.flags == 0)
    {
        ShowCursor(true);
    }
}

void Camera::Init(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_DXUTCamera.SetViewParams(worldPosition, lookAtWorldPosition);
    m_DXUTCamera.SetProjParams(fov, aspectRatio, nearPlane, farPlane);
    m_DXUTCamera.SetRotateButtons(false, false, false, true);
    m_DXUTCamera.SetResetCursorAfterMove(true);
    m_DXUTCamera.SetScalers(0.01f, 500.0f);
    ShowCursor(false);
}

Matrix4x4 Camera::GetViewMatrix() const 
{ 
    return m_DXUTCamera.GetViewMatrix();
}

Matrix4x4 Camera::GetProjMatrix() const 
{ 
    return m_DXUTCamera.GetProjMatrix();
}

Vector3 Camera::GetWorldPosition() const
{
    Vector3 posWS;
    DirectX::XMStoreFloat3(&posWS, m_DXUTCamera.GetEyePt());
    return posWS;
}

Vector3 Camera::GetLookAtDirection() const 
{
    Vector3 lookAtWS;
    DirectX::XMStoreFloat3(&lookAtWS, m_DXUTCamera.GetWorldAhead());
    return lookAtWS;
}

Vector2 Camera::GetViewSpaceZParams() const
{
    float nearClip, farClip;
    GetClipPlanes(nearClip, farClip);
    float rcpNear = 1.0f / nearClip;
    float rcpFar = 1.0f / farClip;

    return Vector2 { (rcpFar - rcpNear), rcpNear };
}

void Camera::GetClipPlanes(float &nearClip, float &farClip) const
{
    nearClip = m_DXUTCamera.GetNearClip();
    farClip = m_DXUTCamera.GetFarClip();
}

void Camera::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_DXUTCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
}

void Camera::FrameMove(float fElapsedTime)
{
    m_DXUTCamera.FrameMove(fElapsedTime);
}
