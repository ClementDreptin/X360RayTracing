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
    Sphere &blueSphere = m_Scene.Spheres[0];
    blueSphere.Position = XMVectorZero();
    blueSphere.Radius = 1.0f;
    blueSphere.Material.Albedo = XMVectorSet(1.0f, 0.0f, 1.0f, 1.0f);

    Sphere &orangeSphere = m_Scene.Spheres[1];
    orangeSphere.Position = XMVectorSet(2.0f, 0.0f, 0.0f, 1.0f);
    orangeSphere.Radius = 1.0f;
    orangeSphere.Material.Albedo = XMVectorSet(0.8f, 0.5f, 0.2f, 1.0f);
    orangeSphere.Material.EmissionColor = m_Scene.Spheres[1].Material.Albedo;
    orangeSphere.Material.EmissionPower = 2.0f;

    Sphere &purpleSphere = m_Scene.Spheres[2];
    purpleSphere.Position = XMVectorSet(0.0f, -101.0f, 0.0f, 1.0f);
    purpleSphere.Radius = 100.0f;
    purpleSphere.Material.Albedo = XMVectorSet(0.2f, 0.3f, 1.0f, 1.0f);
}

HRESULT App::Initialize()
{
    HRESULT hr = S_OK;

    hr = g_Font.Create("game:\\Media\\Fonts\\Arial_16.xpr");
    if (FAILED(hr))
        ATG::FatalError("Couldn't create the font: %x\n", hr);

    hr = m_Renderer.Init();
    XASSERT(SUCCEEDED(hr));

    return hr;
}

HRESULT App::Update()
{
    float ts = static_cast<float>(m_Timer.GetElapsedTime());

    XINPUT_STATE state = {};
    XInputGetState(0, &state);

    bool cameraMoved = m_Camera.Update(state.Gamepad, ts);
    if (cameraMoved)
        m_Renderer.ResetAccumulation();

    return S_OK;
}

HRESULT App::Render()
{
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
