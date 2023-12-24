#include "pch.h"
#include "Renderer/ConstantBuffer.h"

#include "Core/Scene.h"
#include "Renderer/Globals.h"

template<typename T>
ConstantBuffer<T>::ConstantBuffer()
    : m_pBuffer(nullptr)
{
}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer()
{
    if (m_pBuffer != nullptr)
        m_pBuffer->Release();
}

template<typename T>
HRESULT ConstantBuffer<T>::Init()
{
    HRESULT hr = g_pd3dDevice->CreateConstantBuffer(sizeof(T) / sizeof(XMVECTOR), 0, &m_pBuffer);
    if (FAILED(hr))
        Log::Error("Couldn't create constant buffer");

    return hr;
}

template<typename T>
void ConstantBuffer<T>::Update(const T *pData)
{
    D3DVECTOR4 *pConstants = nullptr;
    m_pBuffer->Lock(&pConstants, 0);
    memcpy(pConstants, pData, sizeof(T));
    m_pBuffer->Unlock();
}

template class ConstantBuffer<Scene>;
