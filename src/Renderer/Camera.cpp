#include "pch.h"
#include "Renderer/Camera.h"

#include "Renderer/Globals.h"

Camera::Camera(float verticalFOV, float nearClip, float farClip)
    : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
    m_Position = XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f);
    m_ForwardDirection = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

    m_Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(verticalFOV), ASPECT_RATIO, nearClip, farClip);
    m_InverseProjection = XMMatrixInverse(nullptr, m_Projection);

    RecalculateView();
    RecalculateRayDirections();
}

void Camera::Update(float ts)
{
    // TODO: make the camera move with the controller
}

void Camera::RecalculateView()
{
    m_View = XMMatrixLookAtLH(m_Position, m_Position + m_ForwardDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    m_InverseView = XMMatrixInverse(nullptr, m_View);
}

void Camera::RecalculateRayDirections()
{
    m_RayDirections.resize(IMAGE_WIDTH * IMAGE_HEIGHT);

    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH; x++)
        {
            float normX = static_cast<float>(x) / static_cast<float>(IMAGE_WIDTH);
            float normY = static_cast<float>(y) / static_cast<float>(IMAGE_HEIGHT);
            XMVECTOR coord = XMVectorSet(normX, normY, 0.0f, 0.0f);
            coord = coord * 2.0f - XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

            XMVECTOR target = XMVector4Transform(XMVectorSet(XMVectorGetX(coord), XMVectorGetY(coord), 1.0f, 1.0f), m_InverseProjection);
            float w = XMVectorGetW(target);
            XMVECTOR norm = XMVector4NormalizeEst(target / XMVectorSet(w, w, w, w));
            XMVECTOR rayDirection = XMVector3Transform(norm, m_InverseView);
            m_RayDirections[x + y * IMAGE_WIDTH] = rayDirection;
        }
    }
}
