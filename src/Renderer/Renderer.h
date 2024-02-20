#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/VertexBuffer.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render(const Scene &scene, const Camera &camera);

private:
    VertexBuffer m_VertexBuffer;
    uint32_t m_FrameIndex;

    D3DTexture *m_pAccumulationTexture;
    D3DSurface *m_pRenderTarget;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pImageVertexShader;
    static D3DPixelShader *s_pImagePixelShader;
    static D3DVertexShader *s_pTextureVertexShader;
    static D3DPixelShader *s_pTexturePixelShader;

    void SetCommonState();

    void RenderInAccumulationTexture(const Scene &scene, const Camera &camera);

    void RenderAccumulationTexture();

    static HRESULT InitShaders();
};
