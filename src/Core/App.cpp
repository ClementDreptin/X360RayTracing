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

bool g_ShowControlsTexts = true;

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

    hr = m_Textures.Create("game:\\Media\\Textures\\Textures.xpr");
    if (FAILED(hr))
    {
        Log::Error("Couldn't create textures");
        return hr;
    }

    hr = InitBackground();
    if (FAILED(hr))
        return hr;

    Log::Info("Hello World!");

    return hr;
}

HRESULT App::Update()
{
    return S_OK;
}

HRESULT App::Render()
{
    HRESULT hr = S_OK;

    m_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    hr = RenderBackground();
    if (FAILED(hr))
        return hr;

    hr = RenderFrameRateText();
    if (FAILED(hr))
        return hr;

    hr = g_Console.Render(10.0f, 300.0f);
    if (FAILED(hr))
        return hr;

    m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    return hr;
}

HRESULT App::InitBackground()
{
    HRESULT hr;

    // Get the texture from the bundled resources
    m_pBackgroundTexture = m_Textures.GetTexture("BackgroundTexture");

    // Create the vertices and the vertex buffer
    Vertex vertices[] = {
        Vertex(-1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f), // Bottom Left
        Vertex(-1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f),  // Top Left
        Vertex(1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f),   // Top Right
        Vertex(1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f)   // Bottom Right
    };
    hr = m_VertexBuffer.Init(vertices, ARRAYSIZE(vertices));
    if (FAILED(hr))
        return hr;

    // Create the indices and the index buffer
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    hr = m_pd3dDevice->CreateIndexBuffer(sizeof(indices), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pBackgroundIndexBuffer, nullptr);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the background index buffer");
        return hr;
    }

    // Copy the indices into the index buffer
    void *pIndices = nullptr;
    hr = m_pBackgroundIndexBuffer->Lock(0, sizeof(indices), static_cast<void **>(&pIndices), 0);
    if (FAILED(hr))
    {
        Log::Error("Couldn't lock the background index buffer");
        return hr;
    }
    memcpy(pIndices, indices, sizeof(indices));
    m_pBackgroundIndexBuffer->Unlock();

    // Create the vertex shader
    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Background.xvu", &m_pBackgroundVertexShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the background vertex shader");
        return hr;
    }

    // Create the pixel shader
    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Background.xpu", &m_pBackgroundPixelShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the background pixel shader");
        return hr;
    }

    return S_OK;
}

HRESULT App::RenderBackground()
{
    // Initialize default device states at the start of the frame
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

    // Render the background
    m_pd3dDevice->SetTexture(0, m_pBackgroundTexture);
    m_pd3dDevice->SetVertexDeclaration(m_VertexBuffer.GetVertexDeclaration());
    m_pd3dDevice->SetStreamSource(0, &m_VertexBuffer, 0, sizeof(Vertex));
    m_pd3dDevice->SetVertexShader(m_pBackgroundVertexShader);
    m_pd3dDevice->SetPixelShader(m_pBackgroundPixelShader);
    m_pd3dDevice->SetIndices(m_pBackgroundIndexBuffer);
    m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pBackgroundIndexBuffer->Size / sizeof(DWORD), 0, 2);

    return S_OK;
}

HRESULT App::RenderFrameRateText()
{
    m_Timer.MarkFrame();

    const wchar_t *text = m_Timer.GetFrameRate();

    Text::Props props = { 0 };
    props.X = 10.0f;
    props.Y = g_DisplayHeight - g_Font.GetTextHeight(text) - 10.0f;
    props.Text = text;
    props.Color = D3DCOLOR_XRGB(255, 255, 255);

    return m_FrameRateText.Render(props);
}
