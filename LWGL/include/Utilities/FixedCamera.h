#pragma once

#include "Core/RefCountedObject.h"
#include "3rd/DXUT/Optional/DXUTCamera.h"

namespace lwgl
{
    using namespace core;

    namespace utilities
    {
        class FixedCamera
        {
        public:

            FixedCamera() = default;
            ~FixedCamera() = default;

            void Init(const Vector4 &worldPosition, const Vector4 &lookAtWorldPosition, float fov, float aspectRatio, float nearPlane, float farPlane);

            Matrix4x4   GetViewMatrix() const { return m_ViewMatrix; }
            Matrix4x4   GetProjMatrix() const { return m_ProjectionMatrix; }
            Vector3     GetWorldPosition() const { return m_WorldPosition; }
            Vector2     GetViewSpaceZParams() const { return m_ViewSpaceZParams; }
            void        GetClipPlanes(float &nearClip, float &farClip) const { nearClip = m_NearPlane; farClip = m_FarPlane; }

        private:

            Matrix4x4   m_ViewMatrix {};
            Matrix4x4   m_ProjectionMatrix {};
            Vector3     m_WorldPosition {};
            Vector2     m_ViewSpaceZParams {};
            float       m_NearPlane { 0 };
            float       m_FarPlane { 0 };
        };
    }
}
