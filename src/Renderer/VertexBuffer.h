#pragma once

template<typename T>
class VertexBuffer
{
public:
    VertexBuffer();

    inline D3DVertexBuffer *operator&() const { return m_pBuffer; }

    inline D3DVertexDeclaration *GetVertexDeclaration() const { return m_pVertexDeclaration; }

    HRESULT Init(T *pData, size_t numVertices, D3DVERTEXELEMENT9 *pVertexElements);

    void UpdateBuffer(T *pData, size_t numVertices);

private:
    D3DVertexBuffer *m_pBuffer;
    D3DVertexDeclaration *m_pVertexDeclaration;
};
