#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"
#include "Renderer/Ray.h"

Renderer::Renderer()
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
    Ray ray = {};
    ray.Origin = camera.GetPosition();

    XMCOLOR *pData = m_Image.Lock();
    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH; x++)
        {
            ray.Direction = camera.GetRayDirections()[x + y * IMAGE_WIDTH];
            *(pData++) = TraceRay(scene, ray);
        }
    }
    m_Image.Unlock();

    m_Image.Render();
}

XMCOLOR Renderer::TraceRay(const Scene &scene, const Ray &ray)
{
    if (scene.Spheres.empty())
        return XMCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

    // Math explaination:
    // https://www.youtube.com/watch?v=4NshnkzOdI0
    //
    // Equation of a sphere:
    // (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    // Loop through our sphere to find the closest one to the camera
    Sphere *pClosestSphere = nullptr;
    float hitDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < scene.Spheres.size(); i++)
    {
        const Sphere &sphere = scene.Spheres[i];
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
        if (closestT < hitDistance)
        {
            hitDistance = closestT;
            pClosestSphere = const_cast<Sphere *>(&sphere);
        }
    }

    // Return early if the ray didn't hit any sphere
    if (pClosestSphere == nullptr)
        return XMCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR origin = ray.Origin - pClosestSphere->Position;
    XMVECTOR hitPoint = origin + ray.Direction * hitDistance;

    // Light calculation
    XMVECTOR normal = XMVector3NormalizeEst(hitPoint);
    XMVECTOR lightDirection = XMVector3NormalizeEst(XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f));
    float lightIntensity = std::max<float>(XMVector3Dot(normal, lightDirection * -1.0f).x, 0.0f);

    // Convert colorVec to an XMCOLOR
    XMCOLOR color;
    XMStoreColor(&color, pClosestSphere->Albedo * lightIntensity);

    return color;
}
