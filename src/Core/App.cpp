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
    m_Scene.Spheres[0].Position = XMVectorZero();
    m_Scene.Spheres[0].Radius = 1.0f;
    m_Scene.Spheres[0].Material.Albedo = XMVectorSet(1.0f, 0.0f, 1.0f, 1.0f);
    m_Scene.Spheres[0].Material.Roughness = 1.0f;

    m_Scene.Spheres[1].Position = XMVectorSet(2.0f, 0.0f, 0.0f, 1.0f);
    m_Scene.Spheres[1].Radius = 1.0f;
    m_Scene.Spheres[1].Material.Albedo = XMVectorSet(0.6f, 0.3f, 0.1f, 1.0f);
    m_Scene.Spheres[1].Material.Roughness = 0.8f;
    m_Scene.Spheres[1].Material.EmissionColor = m_Scene.Spheres[1].Material.Albedo;
    m_Scene.Spheres[1].Material.EmissionPower = 1.2f;

    m_Scene.Spheres[2].Position = XMVectorSet(0.0f, -101.0f, 0.0f, 1.0f);
    m_Scene.Spheres[2].Radius = 100.0f;
    m_Scene.Spheres[2].Material.Albedo = XMVectorSet(0.2f, 0.3f, 1.0f, 1.0f);
    m_Scene.Spheres[2].Material.Roughness = 1.0f;
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

    XINPUT_STATE state = {};
    XInputGetState(0, &state);

    m_Camera.Update(state.Gamepad, ts);

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
