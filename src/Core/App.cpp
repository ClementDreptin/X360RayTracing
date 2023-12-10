#include "pch.h"
#include "Core/App.h"

#include "UI/Console.h"
#include "UI/Font.h"

Console g_Console;
Font g_Font;

// The resolution is always 720p, 1080p is created by the hardware scaler
float g_DisplayWidth = 1280.0f;
float g_DisplayHeight = 720.0f;

App::App()
{
}

HRESULT App::Initialize()
{
    HRESULT hr = S_OK;

    hr = g_Font.Create("game:\\Media\\Fonts\\Arial_16.xpr");
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the font");
        return hr;
    }

    m_ImageProps.Width = g_DisplayWidth;
    m_ImageProps.Height = g_DisplayHeight;
    size_t size = static_cast<uint32_t>(m_ImageProps.Width * m_ImageProps.Height);
    m_ImageProps.pData = new D3DCOLOR[size];

    return hr;
}

HRESULT App::Update()
{
    return S_OK;
}

HRESULT App::Render()
{
    m_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    RenderImage();

    RenderFrameRateText();

    g_Console.Render(10, 300);

    m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    return S_OK;
}

D3DCOLOR App::PerPixel(const XMVECTOR &coord)
{
    XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
    XMVECTOR rayDirection = XMVectorSet(coord.x, coord.y, -1.0f, 0.0f);
    float radius = 0.5f;

    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    float a = XMVectorGetX(XMVector4Dot(rayDirection, rayDirection));
    float b = 2.0f * XMVectorGetX(XMVector4Dot(rayOrigin, rayDirection));
    float c = XMVectorGetX(XMVector4Dot(rayOrigin, rayOrigin)) - radius * radius;

    // Quadratic forumula discriminant:
    // b^2 - 4ac

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant >= 0.0f)
        return D3DCOLOR_XRGB(255, 0, 255);

    return D3DCOLOR_XRGB(0, 0, 0);
}

void App::RenderImage()
{
    assert(m_ImageProps.pData != nullptr);
    assert(m_ImageProps.Width > 0.0f && m_ImageProps.Height > 0.0f);

    float aspectRatio = m_ImageProps.Width / m_ImageProps.Height;

    for (uint32_t y = 0; y < m_ImageProps.Height; y++)
    {
        for (uint32_t x = 0; x < m_ImageProps.Width; x++)
        {
            XMVECTOR coord = XMVectorSet(x / m_ImageProps.Width, y / m_ImageProps.Height, 0.0f, 0.0f);
            coord = XMVectorMultiply(coord, XMVectorSet(2.0f, 2.0f, 2.0f, 2.0f));
            coord = XMVectorSubtract(coord, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
            coord = XMVectorSetX(coord, XMVectorGetX(coord) * aspectRatio);

            uint32_t index = static_cast<uint32_t>(x + y * m_ImageProps.Width);
            m_ImageProps.pData[index] = PerPixel(coord);
        }
    }

    m_Image.Render(m_ImageProps);
}

void App::RenderFrameRateText()
{
    m_Timer.MarkFrame();

    const wchar_t *text = m_Timer.GetFrameRate();

    Text::Props props;
    props.X = 10;
    props.Y = g_DisplayHeight - g_Font.GetTextHeight(text) - 10;
    props.Text = text;
    props.Color = D3DCOLOR_XRGB(255, 255, 255);

    m_FrameRateText.Render(props);
}
