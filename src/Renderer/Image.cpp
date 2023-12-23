#include "pch.h"
#include "Renderer/Image.h"

#include "Renderer/Globals.h"

bool Image::s_ShadersInitialized = false;
D3DVertexShader *Image::s_pVertexShader = nullptr;
D3DPixelShader *Image::s_pPixelShader = nullptr;

Image::Image()
{
}

HRESULT Image::Init()
{
    HRESULT hr = S_OK;

    if (!s_ShadersInitialized)
    {
        hr = InitShaders();
        if (FAILED(hr))
            return hr;
    }

    Vertex vertices[] = {
        Vertex(-1.0f, -1.0f, 0.0f), // Bottom Left
        Vertex(-1.0f, 1.0f, 0.0f),  // Top Left
        Vertex(1.0f, 1.0f, 0.0f),   // Top Right
        Vertex(1.0f, -1.0f, 0.0f),  // Bottom Right
    };
    hr = m_VertexBuffer.Init(vertices, ARRAYSIZE(vertices));
    if (FAILED(hr))
        return hr;

    return hr;
}

void Image::Render()
{
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    g_pd3dDevice->SetVertexDeclaration(m_VertexBuffer.GetVertexDeclaration());
    g_pd3dDevice->SetStreamSource(0, &m_VertexBuffer, 0, sizeof(Vertex));
    g_pd3dDevice->SetVertexShader(s_pVertexShader);
    g_pd3dDevice->SetPixelShader(s_pPixelShader);
    g_pd3dDevice->DrawPrimitive(D3DPT_QUADLIST, 0, 1);
}

HRESULT Image::InitShaders()
{
    HRESULT hr = S_OK;

    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Image.xvu", &s_pVertexShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load vertex shader");
        return hr;
    }

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Image.xpu", &s_pPixelShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load pixel shader");
        return hr;
    }

    s_ShadersInitialized = true;

    return hr;
}
