#include "pch.h"
#include "Renderer/VertexBuffer.h"

#include "Renderer/Globals.h"
#include "Renderer/Image.h"

VertexBuffer::VertexBuffer()
    : m_pBuffer(nullptr), m_pVertexDeclaration(nullptr)
{
}

VertexBuffer::~VertexBuffer()
{
    if (m_pBuffer != nullptr)
        m_pBuffer->Release();

    if (m_pVertexDeclaration != nullptr)
        m_pVertexDeclaration->Release();
}

HRESULT VertexBuffer::Init(Vertex *pData, size_t numVertices)
{
    HRESULT hr = S_OK;

    size_t dataSize = sizeof(Vertex) * numVertices;

    hr = g_pd3dDevice->CreateVertexBuffer(dataSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_pBuffer, nullptr);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the vertex buffer");
        return hr;
    }

    D3DVERTEXELEMENT9 vertexElements[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration(vertexElements, &m_pVertexDeclaration);

    UpdateBuffer(pData, numVertices);

    return hr;
}

void VertexBuffer::UpdateBuffer(Vertex *pData, size_t numVertices)
{
    assert(m_pBuffer != nullptr);

    size_t dataSize = sizeof(Vertex) * numVertices;
    void *pVertices = nullptr;

    m_pBuffer->Lock(0, dataSize, &pVertices, 0);
    memcpy(pVertices, pData, dataSize);
    m_pBuffer->Unlock();
}
