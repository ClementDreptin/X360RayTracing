#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"

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

void Renderer::Render()
{
    XMCOLOR *pData = m_Image.Lock();
    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH; x++)
        {
            float normX = static_cast<float>(x) / static_cast<float>(IMAGE_WIDTH);
            float normY = static_cast<float>(y) / static_cast<float>(IMAGE_HEIGHT);
            XMVECTOR coord = XMVectorSet(normX, normY, 0.0f, 0.0f);
            coord = coord * 2.0f - XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            coord = XMVectorSetX(coord, XMVectorGetX(coord) * ASPECT_RATIO);

            *(pData++) = PerPixel(coord);
        }
    }
    m_Image.Unlock();

    m_Image.Render();
}

XMCOLOR Renderer::PerPixel(const XMVECTOR &coord)
{
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

    XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR rayDirection = XMVectorSet(XMVectorGetX(coord), XMVectorGetY(coord), -1.0f, 0.0f);
    float radius = 0.5f;

    float a = XMVectorGetX(XMVector3Dot(rayDirection, rayDirection));
    float b = 2.0f * XMVectorGetX(XMVector3Dot(rayOrigin, rayDirection));
    float c = XMVectorGetX(XMVector3Dot(rayOrigin, rayOrigin)) - radius * radius;

    // Quadratic forumula discriminant:
    // b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return XMCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    // Quadratic formula solutions:
    // (-b +- sqrt(discriminant)) / 2a
    //
    // The "-" solution is always smaller so it is always the closest hit
    // distance, no need to calculate the "+" solution
    float closestT = (-b - sqrtf(discriminant)) / (2.0f * a);
    XMVECTOR hitPoint = rayOrigin + rayDirection * closestT;

    // Light calculation
    XMVECTOR normal = XMVector3NormalizeEst(hitPoint);
    XMVECTOR lightDirection = XMVector3NormalizeEst(XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f));
    float angle = std::max<float>(XMVectorGetX(XMVector3Dot(normal, lightDirection * -1.0f)), 0.0f);
    XMVECTOR colorVec = XMVectorSet(1.0f, 0.0f, 1.0f, 1.0f);
    colorVec = colorVec * angle;

    // Convert colorVec to an XMCOLOR
    XMCOLOR color;
    XMStoreColor(&color, colorVec);

    return color;
}
