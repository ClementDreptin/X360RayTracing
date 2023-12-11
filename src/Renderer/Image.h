#pragma once

#include "Renderer/VertexBuffer.h"

struct ImageVertex
{
    ImageVertex(float x, float y, float z, float u, float v)
        : Pos(x, y, z), TexCoord(u, v) {}

    XMFLOAT3 Pos;
    XMFLOAT2 TexCoord;
};

class Image
{
public:
    Image();

    ~Image();

    HRESULT Init();

    D3DCOLOR *Lock();

    void Unlock();

    void Render();

private:
    bool m_Initialized;

    D3DTexture *m_pTexture;
    VertexBuffer<ImageVertex> m_VertexBuffer;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pVertexShader;
    static D3DPixelShader *s_pPixelShader;

    XMMATRIX m_WorldMatrix;
    XMMATRIX m_ViewMatrix;
    XMMATRIX m_ProjectionMatrix;
    XMMATRIX m_WVPMatrix;

    static HRESULT InitShaders();
};
