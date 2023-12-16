#include "pch.h"
#include "Core/App.h"

#include "Renderer/Globals.h"
#include "UI/Console.h"
#include "UI/Font.h"

Console g_Console;
Font g_Font;

App::App()
    : m_Camera(45.0f, 0.1f, 100.0f)
{
    {
        Sphere sphere;
        sphere.Position = XMVectorZero();
        sphere.Radius = 0.5f;
        sphere.Albedo = XMVectorSet(1.0f, 0.0f, 1.0f, 1.0f);
        m_Scene.Spheres.push_back(sphere);
    }

    {
        Sphere sphere;
        sphere.Position = XMVectorSet(1.0f, 0.0f, -5.0f, 1.0f);
        sphere.Radius = 1.5f;
        sphere.Albedo = XMVectorSet(0.2f, 0.3f, 1.0f, 1.0f);
        m_Scene.Spheres.push_back(sphere);
    }
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

    hr = m_Renderer.Init();
    if (FAILED(hr))
        return hr;

    return hr;
}

HRESULT App::Update()
{
    float ts = static_cast<float>(m_Timer.GetElapsedTime());

    m_Camera.Update(ts);

    return S_OK;
}

HRESULT App::Render()
{
    m_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    m_Renderer.Render(m_Scene, m_Camera);

    RenderFrameRateText();

    g_Console.Render(10, 300);

    m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    return S_OK;
}

void App::RenderFrameRateText()
{
    m_Timer.MarkFrame();

    const wchar_t *text = m_Timer.GetFrameRate();

    Text::Props props;
    props.X = 10;
    props.Y = DISPLAY_HEIGHT - g_Font.GetTextHeight(text) - 10;
    props.Text = text;
    props.Color = XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    m_FrameRateText.Render(props);
}
