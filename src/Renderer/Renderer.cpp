#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"
#include "Renderer/Ray.h"

#define BOUNCES 2

Renderer::Renderer()
    : m_pActiveScene(nullptr), m_pActiveCamera(nullptr)
{
}

HRESULT Renderer::Init()
{
    HRESULT hr = S_OK;

    hr = m_Image.Init();
    if (FAILED(hr))
        Log::Error("Couldn't initialized the image");

    return hr;
}

void Renderer::Render(const Scene &scene, const Camera &camera)
{
    m_pActiveScene = &scene;
    m_pActiveCamera = &camera;

    XMCOLOR *pData = m_Image.Lock();
    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
        for (uint32_t x = 0; x < IMAGE_WIDTH; x++)
            *(pData++) = PerPixel(x, y);
    m_Image.Unlock();

    m_Image.Render();
}

XMCOLOR Renderer::PerPixel(uint32_t x, uint32_t y)
{
    assert(m_pActiveScene != nullptr);
    assert(m_pActiveCamera != nullptr);

    Ray ray;
    ray.Origin = m_pActiveCamera->GetPosition();
    ray.Direction = m_pActiveCamera->GetRayDirections()[x + y * IMAGE_WIDTH];

    XMVECTOR color = XMVectorZero();
    float multiplier = 1.0f;

    // Make the ray bounce to calculate reflection
    for (size_t i = 0; i < BOUNCES; i++)
    {
        // See if the ray hit anything, if not, return the sky color
        HitPayload payload = TraceRay(ray);
        if (payload.HitDistance < 0.0f)
        {
            XMVECTOR skyColor = XMVectorZero();
            color = color + skyColor * multiplier;
            break;
        }

        // Calcuate the pixel color based on the light's position
        XMVECTOR lightDirection = XMVector3NormalizeEst(XMVectorSet(-1.0f, -1.0f, -1.0f, 1.0f));
        float lightIntensity = std::max<float>(XMVector3Dot(payload.WorldNormal, lightDirection * -1.0f).x, 0.0f);
        XMVECTOR sphereColor = m_pActiveScene->Spheres[payload.ObjectIndex].Albedo * lightIntensity;
        color = color + sphereColor * multiplier;

        // Slowly decrease the multiplier over time to prevent the reflection from
        // getting brighter on each bounce
        multiplier *= 0.7f;

        // Once the ray hit the sphere, update its origin to be the hit point
        // right in front of the sphere along the normal. We don't make it exactly
        // the hit position because the hit position might be a little bit inside
        // the sphere due to floating point precision. Which would cause the ray to
        // hit the same sphere again but from the inside.
        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        ray.Direction = XMVector4Reflect(ray.Direction, payload.WorldNormal);
    }

    // Convert the XMVECTOR color to a XMCOLOR color
    XMCOLOR _color;
    XMStoreColor(&_color, color);

    return _color;
}

Renderer::HitPayload Renderer::TraceRay(const Ray &ray)
{
    assert(m_pActiveScene != nullptr);

    // Find the closest sphere along the ray
    uint32_t closestSphereIndex = std::numeric_limits<uint32_t>::max();
    float hitDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < m_pActiveScene->Spheres.size(); i++)
    {
        // Math explaination:
        // https://www.youtube.com/watch?v=4NshnkzOdI0
        //
        // Equation of a sphere (which is a quadratic equation):
        // (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 - r^2) = 0
        // (         a        )t^2 + (          b          )t + (            c           ) = 0
        //
        // where
        // a = ray origin
        // b = ray direction
        // r = radius
        // t = hit distance

        const Sphere &sphere = m_pActiveScene->Spheres[i];
        XMVECTOR origin = ray.Origin - sphere.Position;
        float a = XMVector3Dot(ray.Direction, ray.Direction).x;
        float b = 2.0f * XMVector3Dot(origin, ray.Direction).x;
        float c = XMVector3Dot(origin, origin).x - sphere.Radius * sphere.Radius;

        // Quadratic forumula discriminant:
        // b^2 - 4ac
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
            continue;

        // Quadratic formula solutions:
        // (-b +- sqrt(discriminant)) / 2a
        //
        // The "-" solution is always smaller so it is always the closest hit
        // distance, no need to calculate the "+" solution
        float closestT = (-b - sqrtf(discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphereIndex = i;
        }
    }

    // No sphere was hit
    if (closestSphereIndex == std::numeric_limits<uint32_t>::max())
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphereIndex);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray &ray, float hitDistance, uint32_t objectIndex)
{
    assert(m_pActiveScene != nullptr);

    const Sphere &closestSphere = m_pActiveScene->Spheres[objectIndex];

    HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;
    payload.WorldPosition = ray.Origin + ray.Direction * hitDistance;
    payload.WorldNormal = XMVector3NormalizeEst(payload.WorldPosition - closestSphere.Position);

    return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray &ray)
{
    HitPayload payload;
    payload.HitDistance = -1.0f;

    return payload;
}
