#pragma once

class IndexBuffer
{
public:
    IndexBuffer();

    inline D3DIndexBuffer *operator&() const { return m_pBuffer; }

    HRESULT Init(uint16_t *pData, size_t numIndices);

    void UpdateBuffer(uint16_t *pData, size_t numIndices);

private:
    D3DIndexBuffer *m_pBuffer;
};
