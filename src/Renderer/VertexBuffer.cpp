#include "pch.h"
#include "Renderer/VertexBuffer.h"

#include "Renderer/D3DDevice.h"

VertexBuffer::VertexBuffer()
    : m_pBuffer(nullptr), m_pVertexDeclaration(nullptr)
{
}

HRESULT VertexBuffer::Init(Vertex *pData, size_t numVertices)
{
    HRESULT hr = S_OK;

    size_t dataSize = sizeof(Vertex) * numVertices;

    // Create the vertex buffer
    hr = g_pd3dDevice->CreateVertexBuffer(dataSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_pBuffer, nullptr);
    if (FAILED(hr))
    {
        Log::Error("Couldn't create the vertex buffer");
        return hr;
    }

    // Create a vertex declaration from the element descriptions
    D3DVERTEXELEMENT9 vertexElements[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, sizeof(XMFLOAT3), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        { 0, sizeof(XMFLOAT3) + sizeof(D3DCOLOR), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration(vertexElements, &m_pVertexDeclaration);

    // Copy the data into the vertex buffer
    UpdateBuffer(pData, numVertices);

    return hr;
}

void VertexBuffer::UpdateBuffer(Vertex *pData, size_t numVertices)
{
    assert(m_pBuffer != nullptr);

    size_t dataSize = sizeof(Vertex) * numVertices;
    void *pVertices = nullptr;

    // Copy the vertices into the vertex buffer
    m_pBuffer->Lock(0, dataSize, &pVertices, 0);
    memcpy(pVertices, pData, dataSize);
    m_pBuffer->Unlock();
}
