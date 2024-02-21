#include "pch.h"
#include "Renderer/Camera.h"

#include "Renderer/Globals.h"

Camera::Camera(float verticalFOV, float nearClip, float farClip)
    : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
    m_Position = XMVectorSet(-1.0f, 1.0f, 5.0f, 1.0f);
    m_ForwardDirection = XMVectorSet(0.3f, -0.2f, -1.0f, 1.0f);
    m_UpDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    m_Projection = XMMatrixPerspectiveFovRH(XMConvertToRadians(verticalFOV), ASPECT_RATIO, nearClip, farClip);
    m_InverseProjection = XMMatrixInverse(nullptr, m_Projection);

    RecalculateView();
}

bool Camera::Update(const XINPUT_GAMEPAD &gamepad, float ts)
{
    bool moved = false;

    float speed = GetTranslationSpeed();
    XMVECTOR rightDirection = XMVector3Cross(m_ForwardDirection, m_UpDirection);

    float leftX = ConvertThumbstickValue(gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    float leftY = ConvertThumbstickValue(gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    float rightX = ConvertThumbstickValue(gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    float rightY = ConvertThumbstickValue(gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

    if (gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) // Up
    {
        m_Position += m_UpDirection * speed * ts;
        moved = true;
    }
    else if (gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) // Down
    {
        m_Position -= m_UpDirection * speed * ts;
        moved = true;
    }
    else if (leftX < 0.0f) // Left
    {
        m_Position -= rightDirection * speed * ts;
        moved = true;
    }
    else if (leftX > 0.0f) // Right
    {
        m_Position += rightDirection * speed * ts;
        moved = true;
    }
    else if (leftY > 0.0f) // Forward
    {
        m_Position += m_ForwardDirection * speed * ts;
        moved = true;
    }
    else if (leftY < 0.0f) // Backwards
    {
        m_Position -= m_ForwardDirection * speed * ts;
        moved = true;
    }

    // Rotation
    if (rightX != 0.0f || rightY != 0.0f)
    {
        float rotationSpeed = 0.4f;
        float pitch = rightY * rotationSpeed * ts;
        float yaw = rightX * rotationSpeed * ts;
        XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(-pitch, -yaw, 0.0f);
        m_ForwardDirection = XMVector3Rotate(m_ForwardDirection, rotation);

        moved = true;
    }

    if (moved)
        RecalculateView();

    return moved;
}

void Camera::RecalculateView()
{
    m_View = XMMatrixLookAtRH(m_Position, m_Position + m_ForwardDirection, m_UpDirection);
    m_InverseView = XMMatrixInverse(nullptr, m_View);
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
