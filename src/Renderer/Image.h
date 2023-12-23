#pragma once

#include "Renderer/VertexBuffer.h"

class Image
{
public:
    Image();

    HRESULT Init();

    void Render();

private:
    VertexBuffer m_VertexBuffer;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pVertexShader;
    static D3DPixelShader *s_pPixelShader;

    static HRESULT InitShaders();
};
