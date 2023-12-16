#pragma once

class Camera
{
public:
    Camera(float verticalFOV, float nearClip, float farClip);

    void Update(float ts);

    inline const XMMATRIX &GetProjection() const { return m_Projection; }

    inline const XMMATRIX &GetInverseProjection() const { return m_InverseProjection; }

    inline const XMMATRIX &GetView() const { return m_View; }

    inline const XMMATRIX &GetInverseView() const { return m_InverseView; }

    inline const XMVECTOR &GetPosition() const { return m_Position; }

    inline const XMVECTOR &GetDirection() const { return m_ForwardDirection; }

    inline const std::vector<XMVECTOR> &GetRayDirections() const { return m_RayDirections; }

    inline float GetRotationSpeed() const { return 0.3f; }

private:
    XMMATRIX m_Projection;
    XMMATRIX m_View;
    XMMATRIX m_InverseProjection;
    XMMATRIX m_InverseView;

    float m_VerticalFOV;
    float m_NearClip;
    float m_FarClip;

    XMVECTOR m_Position;
    XMVECTOR m_ForwardDirection;

    std::vector<XMVECTOR> m_RayDirections;

    void RecalculateView();

    void RecalculateRayDirections();

    static float ConvertThumbstickValue(int16_t thumbstickValue, int16_t deadZone);
};
