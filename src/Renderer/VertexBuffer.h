#pragma once

struct Vertex
{
    Vertex(float x, float y, float z, float u, float v)
        : Pos(x, y, z), TexCoord(u, v) {}

    XMFLOAT3 Pos;
    XMFLOAT2 TexCoord;
};

class VertexBuffer
{
public:
    VertexBuffer();

    ~VertexBuffer();

    inline D3DVertexBuffer *operator&() const { return m_pBuffer; }

    inline D3DVertexDeclaration *GetVertexDeclaration() const { return m_pVertexDeclaration; }

    HRESULT Init(Vertex *pData, size_t numVertices);

    void Update(const Vertex *pData, size_t numVertices);

private:
    D3DVertexBuffer *m_pBuffer;
    D3DVertexDeclaration *m_pVertexDeclaration;
};
