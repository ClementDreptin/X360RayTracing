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
    struct Props
    {
        Props()
            : X(0), Y(0), Width(0), Height(0), pData(nullptr) {}

        uint32_t X;
        uint32_t Y;
        uint32_t Width;
        uint32_t Height;
        D3DCOLOR *pData;
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

    void PopulateTexture();
};
