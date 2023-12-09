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

    m_BackgroundProps.Width = ROUND(g_DisplayWidth);
    m_BackgroundProps.Height = ROUND(g_DisplayHeight);
    m_BackgroundProps.pData = new D3DCOLOR[m_BackgroundProps.Width * m_BackgroundProps.Height];

    return hr;
}

HRESULT App::Update()
{
    return S_OK;
}

HRESULT App::Render()
{
    m_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    RenderBackground();

    RenderFrameRateText();

    g_Console.Render(10, 300);

    m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    return S_OK;
}

void App::RenderBackground()
{
    assert(m_BackgroundProps.pData != nullptr);
    assert(m_BackgroundProps.Width > 0 && m_BackgroundProps.Height > 0);

    for (uint32_t y = 0; y < m_BackgroundProps.Height; y++)
        for (uint32_t x = 0; x < m_BackgroundProps.Width; x++)
            m_BackgroundProps.pData[x + y * m_BackgroundProps.Width] = D3DCOLOR_XRGB(255, 0, 0);

    m_Background.Render(m_BackgroundProps);
}

void App::RenderFrameRateText()
{
    m_Timer.MarkFrame();

    const wchar_t *text = m_Timer.GetFrameRate();

    Text::Props props;
    props.X = 10;
    props.Y = static_cast<uint32_t>(g_DisplayHeight - g_Font.GetTextHeight(text) - 10);
    props.Text = text;
    props.Color = D3DCOLOR_XRGB(255, 255, 255);

    m_FrameRateText.Render(props);
}
