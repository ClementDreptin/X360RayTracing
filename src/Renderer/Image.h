#pragma once

#include "Renderer/PixelShader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexShader.h"

struct ImageVertex
{
    ImageVertex() {}

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

    Texture m_Texture;
    VertexBuffer<ImageVertex> m_VertexBuffer;

    static bool s_ShadersInitialized;
    static VertexShader s_VertexShader;
    static PixelShader s_PixelShader;

    XMMATRIX m_WorldMatrix;
    XMMATRIX m_ViewMatrix;
    XMMATRIX m_ProjectionMatrix;
    XMMATRIX m_WVPMatrix;

    HRESULT Init();

    static HRESULT InitShaders();

    void CalculateWorldViewProjectionMatrix();

    void UpdateVertexBuffer();
};
