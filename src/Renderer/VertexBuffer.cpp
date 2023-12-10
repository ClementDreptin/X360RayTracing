#include "pch.h"
#include "Renderer/VertexBuffer.h"

#include "Renderer/Globals.h"
#include "Renderer/Image.h"

template<typename T>
VertexBuffer<T>::VertexBuffer()
    : m_pBuffer(nullptr), m_pVertexDeclaration(nullptr)
{
}

template<typename T>
VertexBuffer<T>::~VertexBuffer()
{
    if (m_pBuffer != nullptr)
        m_pBuffer->Release();

    if (m_pVertexDeclaration != nullptr)
        m_pVertexDeclaration->Release();
}

template<typename T>
HRESULT VertexBuffer<T>::Init(T *pData, size_t numVertices, D3DVERTEXELEMENT9 *pVertexElements)
{
    HRESULT hr = S_OK;

    size_t dataSize = sizeof(T) * numVertices;

    hr = g_pd3dDevice->CreateVertexBuffer(dataSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_pBuffer, nullptr);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the vertex buffer");
        return hr;
    }

    g_pd3dDevice->CreateVertexDeclaration(pVertexElements, &m_pVertexDeclaration);

    UpdateBuffer(pData, numVertices);

    return hr;
}

template<typename T>
void VertexBuffer<T>::UpdateBuffer(T *pData, size_t numVertices)
{
    assert(m_pBuffer != nullptr);

    size_t dataSize = sizeof(T) * numVertices;
    void *pVertices = nullptr;

    m_pBuffer->Lock(0, dataSize, &pVertices, 0);
    memcpy(pVertices, pData, dataSize);
    m_pBuffer->Unlock();
}

template class VertexBuffer<ImageVertex>;
