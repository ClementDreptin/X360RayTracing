#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"

bool Renderer::s_ShadersInitialized = false;
D3DVertexShader *Renderer::s_pImageVertexShader = nullptr;
D3DPixelShader *Renderer::s_pImagePixelShader = nullptr;
D3DVertexShader *Renderer::s_pTextureVertexShader = nullptr;
D3DPixelShader *Renderer::s_pTexturePixelShader = nullptr;

Renderer::Renderer()
    : m_FrameIndex(1), m_pAccumulationTexture(nullptr), m_pRenderTarget(nullptr)
{
}

HRESULT Renderer::Init()
{
    HRESULT hr = S_OK;

    if (!s_ShadersInitialized)
    {
        hr = InitShaders();
        if (FAILED(hr))
            return hr;
    }

    Vertex vertices[] = {
        Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f), // Top Left
        Vertex(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f), // Bottom Left
        Vertex(+1.0f, +1.0f, 0.0f, 1.0f, 0.0f), // Bottom Right
        Vertex(+1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // Top Right
    };
    hr = m_VertexBuffer.Init(vertices, ARRAYSIZE(vertices));
    if (FAILED(hr))
        return hr;

    hr = g_pd3dDevice->CreateTexture(
        TEXTURE_WIDTH,
        TEXTURE_HEIGHT,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pAccumulationTexture,
        nullptr
    );
    if (FAILED(hr))
    {
        Log::Error("Couldn't create accumulation texture");
        return hr;
    }

    hr = g_pd3dDevice->CreateRenderTarget(
        TEXTURE_WIDTH,
        TEXTURE_HEIGHT,
        D3DFMT_A8R8G8B8,
        D3DMULTISAMPLE_NONE,
        0,
        FALSE,
        &m_pRenderTarget,
        nullptr
    );
    if (FAILED(hr))
    {
        Log::Error("Couldn't create render target");
        return hr;
    }

    return hr;
}

void Renderer::Render(const Scene &scene, const Camera &camera)
{
    SetCommonState();

    AccumulateInTexture(scene, camera);

    RenderAccumulationTexture();

    m_FrameIndex++;
}

void Renderer::SetCommonState()
{
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    g_pd3dDevice->SetVertexDeclaration(m_VertexBuffer.GetVertexDeclaration());
    g_pd3dDevice->SetStreamSource(0, &m_VertexBuffer, 0, sizeof(Vertex));

    float frameIndex = static_cast<float>(m_FrameIndex);
    g_pd3dDevice->SetPixelShaderConstantF(0, reinterpret_cast<const float *>(&frameIndex), 1);

    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    g_pd3dDevice->SetTexture(0, m_pAccumulationTexture);
}

void Renderer::AccumulateInTexture(const Scene &scene, const Camera &camera)
{
    D3DSurface *pRenderTarget0;
    g_pd3dDevice->GetRenderTarget(0, &pRenderTarget0);

    g_pd3dDevice->SetRenderTarget(0, m_pRenderTarget);

    g_pd3dDevice->SetVertexShader(s_pImageVertexShader);
    g_pd3dDevice->SetPixelShader(s_pImagePixelShader);
    g_pd3dDevice->SetPixelShaderConstantF(1, reinterpret_cast<const float *>(&camera.GetPosition()), 1);
    g_pd3dDevice->SetPixelShaderConstantF(4, reinterpret_cast<const float *>(&camera.GetInverseProjection()), 4);
    g_pd3dDevice->SetPixelShaderConstantF(8, reinterpret_cast<const float *>(&camera.GetInverseView()), 4);
    g_pd3dDevice->SetPixelShaderConstantF(12, reinterpret_cast<const float *>(&scene), sizeof(Scene) / sizeof(XMVECTOR));
    g_pd3dDevice->DrawPrimitive(D3DPT_QUADLIST, 0, 1);

    g_pd3dDevice->Resolve(D3DRESOLVE_RENDERTARGET0, nullptr, m_pAccumulationTexture, nullptr, 0, 0, nullptr, 0.0f, 0, nullptr);

    g_pd3dDevice->SetRenderTarget(0, pRenderTarget0);
    pRenderTarget0->Release();
}

void Renderer::RenderAccumulationTexture()
{
    g_pd3dDevice->SetVertexShader(s_pTextureVertexShader);
    g_pd3dDevice->SetPixelShader(s_pTexturePixelShader);

    g_pd3dDevice->DrawPrimitive(D3DPT_QUADLIST, 0, 1);
}

HRESULT Renderer::InitShaders()
{
    HRESULT hr = S_OK;

    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Image.xvu", &s_pImageVertexShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load image vertex shader");
        return hr;
    }

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Image.xpu", &s_pImagePixelShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load image pixel shader");
        return hr;
    }

    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Texture.xvu", &s_pTextureVertexShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load texture vertex shader");
        return hr;
    }

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Texture.xpu", &s_pTexturePixelShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load texture pixel shader");
        return hr;
    }

    s_ShadersInitialized = true;

    return hr;
}
