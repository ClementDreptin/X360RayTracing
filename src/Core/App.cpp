#include "pch.h"
#include "Core/App.h"

#include <AtgUtil.h>

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

    m_ImageProps.Width = ROUND(g_DisplayWidth);
    m_ImageProps.Height = ROUND(g_DisplayHeight);
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

D3DCOLOR App::PerPixel(float x, float y)
{
    uint8_t r = static_cast<uint8_t>(x * 255.0f);
    uint8_t g = static_cast<uint8_t>(y * 255.0f);

    return D3DCOLOR_XRGB(r, g, 0);
}

void App::RenderImage()
{
    assert(m_ImageProps.pData != nullptr);
    assert(m_ImageProps.Width > 0.0f && m_ImageProps.Height > 0.0f);

    for (uint32_t y = 0; y < m_ImageProps.Height; y++)
    {
        for (uint32_t x = 0; x < m_ImageProps.Width; x++)
        {
            uint32_t index = static_cast<uint32_t>(x + y * m_ImageProps.Width);
            m_ImageProps.pData[index] = PerPixel(x / m_ImageProps.Width, y / m_ImageProps.Height);
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
