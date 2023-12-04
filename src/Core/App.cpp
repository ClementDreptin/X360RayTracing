#include "pch.h"
#include "Core/App.h"

#include <AtgUtil.h>

#include "UI/Console.h"
#include "UI/Font.h"

// The Xbox 360 stores textures in 32x32 tiles in GPU memory so texture
// widths and heights need to be multiples of 32
#define TILE_SIZE 32
#define ROUND(size) ((uint32_t)(size) + TILE_SIZE - 1) & ~(TILE_SIZE - 1)

Console g_Console;
Font g_Font;

// The resolution is always 720p, 1080p is created by the hardware scaler
float g_DisplayWidth = 1280.0f;
float g_DisplayHeight = 720.0f;

App::App()
    : m_BackgroundWidth(0), m_BackgroundHeight(0), m_BackgroundData(nullptr)
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

    m_BackgroundWidth = ROUND(g_DisplayWidth);
    m_BackgroundHeight = ROUND(g_DisplayHeight);
    m_BackgroundData = new D3DCOLOR[m_BackgroundWidth * m_BackgroundHeight];

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

    g_Console.Render(10.0f, 300.0f);

    m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    return S_OK;
}

void App::RenderBackground()
{
    assert(m_BackgroundData != nullptr);
    assert(m_BackgroundWidth > 0 && m_BackgroundHeight > 0);

    for (uint32_t y = 0; y < m_BackgroundHeight; y++)
        for (uint32_t x = 0; x < m_BackgroundWidth; x++)
            m_BackgroundData[x + y * m_BackgroundWidth] = D3DCOLOR_XRGB(255, 0, 0);

    Image::Props props = {};
    props.Width = static_cast<float>(m_BackgroundWidth);
    props.Height = static_cast<float>(m_BackgroundHeight);
    props.pData = m_BackgroundData;

    m_Background.Render(props);
}

void App::RenderFrameRateText()
{
    m_Timer.MarkFrame();

    const wchar_t *text = m_Timer.GetFrameRate();

    Text::Props props = { 0 };
    props.X = 10.0f;
    props.Y = g_DisplayHeight - g_Font.GetTextHeight(text) - 10.0f;
    props.Text = text;
    props.Color = D3DCOLOR_XRGB(255, 255, 255);

    m_FrameRateText.Render(props);
}
