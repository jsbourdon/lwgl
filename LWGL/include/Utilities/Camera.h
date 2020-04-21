#pragma once

#include "Core/RefCountedObject.h"
#include "3rd/DXUT/Optional/DXUTCamera.h"

namespace lwgl
{
    using namespace core;

    namespace utilities
    {
        class Camera : public RefCountedObject<Camera>
        {
            friend base;
            friend class RenderCore;

        public:

            Camera();
            void Init(Vector4 worldPosition, Vector4 lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane);

            Matrix4x4   GetViewMatrix() const;
            Matrix4x4   GetProjMatrix() const;
            Vector3     GetWorldPosition() const;
            Vector3     GetLookAtDirection() const;
            Vector3     GetLookAtPoint() const;
            Vector2     GetViewSpaceZParams() const;
            void        GetClipPlanes(float &near, float &far) const;

        private:

            ~Camera();

            void HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
            void FrameMove(float fElapsedTime);

        private:

            CFirstPersonCamera m_DXUTCamera;
        };
    }
}
