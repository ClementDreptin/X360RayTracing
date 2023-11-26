#pragma once

struct Vertex
{
    Vertex() {}

    Vertex(float x, float y, float z, D3DCOLOR color, float u, float v)
        : Pos(x, y, z), Color(color), TexCoord(u, v) {}

    XMFLOAT3 Pos;
    D3DCOLOR Color;
    XMFLOAT2 TexCoord;
};

class VertexBuffer
{
public:
    VertexBuffer();

    inline D3DVertexBuffer *operator&() const { return m_pBuffer; }

    inline D3DVertexDeclaration *GetVertexDeclaration() const { return m_pVertexDeclaration; }

    HRESULT Init(Vertex *pData, size_t numVertices);

    void UpdateBuffer(Vertex *pData, size_t numVertices);

private:
    D3DVertexBuffer *m_pBuffer;
    D3DVertexDeclaration *m_pVertexDeclaration;
};
