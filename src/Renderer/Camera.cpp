#include "pch.h"
#include "Renderer/Camera.h"

#include "Renderer/Globals.h"

Camera::Camera(float verticalFOV, float nearClip, float farClip)
    : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
    m_Position = XMVectorSet(0.0f, 0.0f, 3.0f, 1.0f);
    m_ForwardDirection = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);

    m_Projection = XMMatrixPerspectiveFovRH(XMConvertToRadians(verticalFOV), ASPECT_RATIO, nearClip, farClip);
    m_InverseProjection = XMMatrixInverse(nullptr, m_Projection);

    RecalculateView();
    RecalculateRayDirections();
}

void Camera::Update(float ts)
{
    bool moved = false;

    float speed = 5.0f;
    XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    XMVECTOR rightDirection = XMVector3Cross(m_ForwardDirection, upDirection);

    XINPUT_STATE state = {};
    XInputGetState(0, &state);

    float leftX = ConvertThumbstickValue(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    float leftY = ConvertThumbstickValue(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    float rightX = ConvertThumbstickValue(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    float rightY = ConvertThumbstickValue(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

    if (leftY > 0.0f) // Up
    {
        m_Position = m_Position + upDirection * speed * ts;
        moved = true;
    }
    else if (leftY < 0.0f) // Down
    {
        m_Position = m_Position - upDirection * speed * ts;
        moved = true;
    }
    else if (leftX < 0.0f) // Left
    {
        m_Position = m_Position - rightDirection * speed * ts;
        moved = true;
    }
    else if (leftX > 0.0f) // Right
    {
        m_Position = m_Position + rightDirection * speed * ts;
        moved = true;
    }
    else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) // Forward
    {
        m_Position = m_Position + m_ForwardDirection * speed * ts;
        moved = true;
    }
    else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) // Backwards
    {
        m_Position = m_Position - m_ForwardDirection * speed * ts;
        moved = true;
    }

    // Rotation
    if (rightX != 0.0f || rightY != 0.0f)
    {
        // TODO: fix the camera rotating with an offset instead of around the sphere

        float pitch = rightY * GetRotationSpeed();
        float yaw = rightX * GetRotationSpeed();
        XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(-pitch, -yaw, 0.0f);
        m_ForwardDirection = XMVector3Rotate(m_ForwardDirection, rotation);

        moved = true;
    }

    if (moved)
    {
        RecalculateView();
        RecalculateRayDirections();
    }
}

void Camera::RecalculateView()
{
    m_View = XMMatrixLookAtRH(m_Position, m_Position + m_ForwardDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
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

            XMVECTOR target = XMVector4Transform(XMVectorSet(coord.x, coord.y, 1.0f, 1.0f), m_InverseProjection);
            XMVECTOR norm = XMVector4NormalizeEst(target / XMVectorSet(target.w, target.w, target.w, target.w));
            XMVECTOR rayDirection = XMVector4Transform(norm, m_InverseView);
            m_RayDirections[x + y * IMAGE_WIDTH] = rayDirection;
        }
    }
}

float Camera::ConvertThumbstickValue(int16_t thumbstickValue, int16_t deadZone)
{
    // Convert thumbstick values coming from XINPUT to a [-1;+1] space

    if (thumbstickValue > +deadZone)
        return (thumbstickValue - deadZone) / (32767.0f - deadZone);
    if (thumbstickValue < -deadZone)
        return (thumbstickValue + deadZone + 1.0f) / (32767.0f - deadZone);

    return 0.0f;
}
