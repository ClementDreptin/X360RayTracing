#pragma once

#include "Renderer/VertexBuffer.h"

struct ImageVertex
{
    ImageVertex(float x, float y, float z, D3DCOLOR color, float u, float v)
        : Pos(x, y, z), Color(color), TexCoord(u, v) {}

    XMFLOAT3 Pos;
    D3DCOLOR Color;
    XMFLOAT2 TexCoord;
};

class Image
{
public:
    struct Props
    {
        float X;
        float Y;
        float Width;
        float Height;
        std::string TextureFilePath;
        D3DCOLOR TintColor;
    };

    Image();

    void Render(const Props &props);

private:
    Props m_Props;
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

    HRESULT Init();

    static HRESULT InitShaders();

    void CalculateWorldViewProjectionMatrix();

    void UpdateVertexBuffer();
};
