#pragma once

#include "Renderer/VertexBuffer.h"

struct ImageVertex
{
    ImageVertex(float x, float y, float z)
        : Pos(x, y, z)
    {
    }

    XMFLOAT3 Pos;
};

class Image
{
public:
    Image();

    HRESULT Init();

    void Render();

private:
    VertexBuffer<ImageVertex> m_VertexBuffer;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pVertexShader;
    static D3DPixelShader *s_pPixelShader;

    static HRESULT InitShaders();
};
