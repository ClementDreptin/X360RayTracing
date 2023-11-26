#include "pch.h"
#include "Renderer/IndexBuffer.h"

#include "Renderer/D3DDevice.h"

IndexBuffer::IndexBuffer()
    : m_pBuffer(nullptr)
{
}

HRESULT IndexBuffer::Init(uint16_t *pData, size_t numIndices)
{
    HRESULT hr = S_OK;

    size_t dataSize = sizeof(uint16_t) * numIndices;

    // Create the index buffer
    hr = g_pd3dDevice->CreateIndexBuffer(dataSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 0, &m_pBuffer, nullptr);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the index buffer");
        return hr;
    }

    // Copy the data into the index buffer
    UpdateBuffer(pData, numIndices);

    return hr;
}

void IndexBuffer::UpdateBuffer(uint16_t *pData, size_t numIndices)
{
    assert(m_pBuffer != nullptr);

    size_t dataSize = sizeof(uint16_t) * numIndices;
    void *pIndices = nullptr;

    // Copy the indices into the index buffer
    m_pBuffer->Lock(0, dataSize, &pIndices, 0);
    memcpy(pIndices, pData, dataSize);
    m_pBuffer->Unlock();
}
