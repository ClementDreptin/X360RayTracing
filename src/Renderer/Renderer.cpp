#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"

namespace Utils
{

static uint32_t PcgHash(uint32_t input)
{
    uint32_t state = input * 747796405 + 2891336453;
    uint32_t word = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;

    return (word >> 22) ^ word;
}

static uint32_t RandomUint(uint32_t &seed)
{
    seed = PcgHash(seed);

    return seed;
}

}

bool Renderer::s_ShadersInitialized = false;
D3DVertexShader *Renderer::s_pImageVertexShader = nullptr;
D3DPixelShader *Renderer::s_pImagePixelShader = nullptr;
D3DVertexShader *Renderer::s_pTextureVertexShader = nullptr;
D3DPixelShader *Renderer::s_pTexturePixelShader = nullptr;

Renderer::Renderer()
    : m_FrameIndex(1), m_RandomSeed(0), m_pRandomTexture(nullptr)
{
    ZeroMemory(m_pAccumulationTextures, sizeof(m_pAccumulationTextures));
    ZeroMemory(m_pRenderTargets, sizeof(m_pRenderTargets));
}

HRESULT Renderer::Init()
{
    HRESULT hr = S_OK;

    if (!s_ShadersInitialized)
    {
        hr = InitShaders();
        XASSERT(SUCCEEDED(hr));
    }

    Vertex vertices[] = {
        Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f), // Bottom Left
        Vertex(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f), // Top Left
        Vertex(+1.0f, +1.0f, 0.0f, 1.0f, 0.0f), // Top Right
        Vertex(+1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // Bottom Right
    };
    hr = m_VertexBuffer.Init(vertices, ARRAYSIZE(vertices));
    XASSERT(SUCCEEDED(hr));

    // The accumulation uses 32-bit floats. With fewer bits, the moving average stops
    // changing correctly once each frame's contribution becomes smaller than the
    // precision (after a few hundred frames with 8 bits, a couple thousands with 16-bit
    // floats), which creates visible layers of light. Floats also prevent colors brighter
    // than 1 from being clamped before being averaged.
    //
    // Each render target is only one tile high, they get resolved into the accumulation
    // textures once per tile. The render targets are placed one after the other in EDRAM.
    uint32_t edramBase = 0;
    for (uint32_t i = 0; i < ACCUMULATION_TARGET_COUNT; i++)
    {
        hr = g_pd3dDevice->CreateTexture(
            TEXTURE_WIDTH,
            TEXTURE_HEIGHT,
            1,
            0,
            D3DFMT_G32R32F,
            D3DPOOL_DEFAULT,
            &m_pAccumulationTextures[i],
            nullptr
        );
        XASSERT(SUCCEEDED(hr));

        D3DSURFACE_PARAMETERS parameters = {};
        parameters.Base = edramBase;
        hr = g_pd3dDevice->CreateRenderTarget(
            TEXTURE_WIDTH,
            TILE_HEIGHT,
            D3DFMT_G32R32F,
            D3DMULTISAMPLE_NONE,
            0,
            FALSE,
            &m_pRenderTargets[i],
            &parameters
        );
        XASSERT(SUCCEEDED(hr));

        edramBase += XGSurfaceSize(TEXTURE_WIDTH, TILE_HEIGHT, D3DFMT_G32R32F, D3DMULTISAMPLE_NONE);
    }

    hr = InitRandomTexture();
    XASSERT(SUCCEEDED(hr));

    return hr;
}

HRESULT Renderer::InitRandomTexture()
{
    // The GPU doesn't support integer operations, which makes good random number generators
    // hard to write in shaders. Instead, random values are generated once on the CPU and stored
    // in a texture, which the pixel shader samples at a random offset that changes every frame
    // and every bounce.
    HRESULT hr = g_pd3dDevice->CreateTexture(
        RANDOM_TEXTURE_SIZE,
        RANDOM_TEXTURE_SIZE,
        1,
        0,
        D3DFMT_LIN_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pRandomTexture,
        nullptr
    );
    if (FAILED(hr))
        return hr;

    D3DLOCKED_RECT rect = {};
    hr = m_pRandomTexture->LockRect(0, &rect, nullptr, 0);
    if (FAILED(hr))
        return hr;

    uint32_t seed = 0;
    for (uint32_t y = 0; y < RANDOM_TEXTURE_SIZE; y++)
    {
        uint32_t *pRow = reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(rect.pBits) + y * rect.Pitch);
        for (uint32_t x = 0; x < RANDOM_TEXTURE_SIZE; x++)
            pRow[x] = Utils::RandomUint(seed);
    }

    m_pRandomTexture->UnlockRect(0);

    return hr;
}

void Renderer::Render(const Scene &scene, const Camera &camera)
{
    SetCommonState();

    RenderInAccumulationTexture(scene, camera);

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

    // 32-bit float textures can't be filtered
    g_pd3dDevice->SetTexture(0, m_pAccumulationTextures[0]);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetTexture(2, m_pAccumulationTextures[1]);
    g_pd3dDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
}

void Renderer::RenderInAccumulationTexture(const Scene &scene, const Camera &camera)
{
    // Get the original render target
    D3DSurface *pOriginalRenderTarget = nullptr;
    g_pd3dDevice->GetRenderTarget(0, &pOriginalRenderTarget);

    // Set the render targets that get resolved to the accumulation textures
    for (uint32_t i = 0; i < ACCUMULATION_TARGET_COUNT; i++)
        g_pd3dDevice->SetRenderTarget(i, m_pRenderTargets[i]);

    // Set shaders and their constants
    XMVECTOR frameIndex = XMVectorReplicate(static_cast<float>(m_FrameIndex));
    g_pd3dDevice->SetVertexShader(s_pImageVertexShader);
    g_pd3dDevice->SetPixelShader(s_pImagePixelShader);
    g_pd3dDevice->SetPixelShaderConstantF(0, reinterpret_cast<const float *>(&frameIndex), 1);
    g_pd3dDevice->SetPixelShaderConstantF(1, reinterpret_cast<const float *>(&camera.GetPosition()), 1);
    g_pd3dDevice->SetPixelShaderConstantF(4, reinterpret_cast<const float *>(&camera.GetInverseProjection()), 4);
    g_pd3dDevice->SetPixelShaderConstantF(8, reinterpret_cast<const float *>(&camera.GetInverseView()), 4);
    g_pd3dDevice->SetPixelShaderConstantF(12, reinterpret_cast<const float *>(&scene), sizeof(Scene) / sizeof(XMVECTOR));

    // Pick a random offset in the random texture for each bounce
    XMVECTOR randomOffsets[BOUNCES] = {};
    for (uint32_t i = 0; i < BOUNCES; i++)
        randomOffsets[i] = XMVectorSet(
            static_cast<float>(Utils::RandomUint(m_RandomSeed) % RANDOM_TEXTURE_SIZE),
            static_cast<float>(Utils::RandomUint(m_RandomSeed) % RANDOM_TEXTURE_SIZE),
            0.0f,
            0.0f
        );
    g_pd3dDevice->SetPixelShaderConstantF(32, reinterpret_cast<const float *>(randomOffsets), BOUNCES);

    g_pd3dDevice->SetTexture(1, m_pRandomTexture);
    g_pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    // Draw with pixel centers at (0.5, 0.5). By default, pixel centers are at integer
    // coordinates (D3D9 convention) so, since the render target and the accumulation
    // texture have the same size, the interpolated texture coordinates land exactly
    // on texel boundaries. Each triangle of the quad then rounds to a different texel,
    // which shifts the upper left triangle by one texel every frame.
    g_pd3dDevice->SetRenderState(D3DRS_HALFPIXELOFFSET, TRUE);

    for (uint32_t i = 0; i < TILE_COUNT; i++)
    {
        // Tell the shader which rows of the image this tile covers
        uint32_t tileOffset = i * TILE_HEIGHT;
        XMVECTOR tileOffsetVector = XMVectorReplicate(static_cast<float>(tileOffset));
        g_pd3dDevice->SetPixelShaderConstantF(2, reinterpret_cast<const float *>(&tileOffsetVector), 1);

        g_pd3dDevice->DrawPrimitive(D3DPT_QUADLIST, 0, 1);

        // Resolve the render targets to the tile's rows in the accumulation textures
        D3DRECT sourceRect = { 0, 0, TEXTURE_WIDTH, TILE_HEIGHT };
        D3DPOINT destPoint = { 0, static_cast<LONG>(tileOffset) };
        uint32_t resolveFlags[ACCUMULATION_TARGET_COUNT] = { D3DRESOLVE_RENDERTARGET0, D3DRESOLVE_RENDERTARGET1 };
        for (uint32_t j = 0; j < ACCUMULATION_TARGET_COUNT; j++)
            g_pd3dDevice->Resolve(
                resolveFlags[j],
                &sourceRect,
                m_pAccumulationTextures[j],
                &destPoint,
                0,
                0,
                nullptr,
                0.0f,
                0,
                nullptr
            );
    }

    g_pd3dDevice->SetRenderState(D3DRS_HALFPIXELOFFSET, FALSE);

    // Restore the original render target
    for (uint32_t i = 1; i < ACCUMULATION_TARGET_COUNT; i++)
        g_pd3dDevice->SetRenderTarget(i, nullptr);
    g_pd3dDevice->SetRenderTarget(0, pOriginalRenderTarget);
    pOriginalRenderTarget->Release();
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
    XASSERT(SUCCEEDED(hr));

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Image.xpu", &s_pImagePixelShader);
    XASSERT(SUCCEEDED(hr));

    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Texture.xvu", &s_pTextureVertexShader);
    XASSERT(SUCCEEDED(hr));

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Texture.xpu", &s_pTexturePixelShader);
    XASSERT(SUCCEEDED(hr));

    s_ShadersInitialized = true;

    return hr;
}
