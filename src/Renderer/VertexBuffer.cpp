#include "pch.h"
#include "Renderer/VertexBuffer.h"

#include "Renderer/Globals.h"

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
    XASSERT(pData != nullptr);
    XASSERT(numVertices > 0);

    HRESULT hr = S_OK;

    size_t dataSize = sizeof(Vertex) * numVertices;

    hr = g_pd3dDevice->CreateVertexBuffer(dataSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_pBuffer, nullptr);
    XASSERT(SUCCEEDED(hr));

    D3DVERTEXELEMENT9 vertexElements[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, sizeof(XMFLOAT3), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration(vertexElements, &m_pVertexDeclaration);

    Update(pData, numVertices);

    return hr;
}

void VertexBuffer::Update(const Vertex *pData, size_t numVertices)
{
    XASSERT(m_pBuffer != nullptr);
    XASSERT(pData != nullptr);
    XASSERT(numVertices > 0);

    size_t dataSize = sizeof(Vertex) * numVertices;
    void *pVertices = nullptr;

    m_pBuffer->Lock(0, dataSize, &pVertices, 0);
    memcpy(pVertices, pData, dataSize);
    m_pBuffer->Unlock();
}
