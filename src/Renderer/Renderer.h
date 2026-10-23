#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Globals.h"
#include "Renderer/VertexBuffer.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render(const Scene &scene, const Camera &camera);

    void ResetAccumulation() { m_FrameIndex = 1; }

private:
    VertexBuffer m_VertexBuffer;
    uint32_t m_FrameIndex;
    uint32_t m_RandomSeed;

    // Index 0 holds the red and green channels, index 1 holds the blue channel
    D3DTexture *m_pAccumulationTextures[ACCUMULATION_TARGET_COUNT];
    D3DSurface *m_pRenderTargets[ACCUMULATION_TARGET_COUNT];
    D3DTexture *m_pRandomTexture;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pImageVertexShader;
    static D3DPixelShader *s_pImagePixelShader;
    static D3DVertexShader *s_pTextureVertexShader;
    static D3DPixelShader *s_pTexturePixelShader;

    HRESULT InitRandomTexture();

    void SetCommonState();

    void RenderInAccumulationTexture(const Scene &scene, const Camera &camera);

    void RenderAccumulationTexture();

    static HRESULT InitShaders();
};
