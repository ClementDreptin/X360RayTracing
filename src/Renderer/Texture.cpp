#include "pch.h"
#include "Renderer/Texture.h"

#include "Renderer/D3DDevice.h"

Texture::Texture()
    : m_pTexture(nullptr)
{
}

HRESULT Texture::Init(const std::string &filePath)
{
    return D3DXCreateTextureFromFile(g_pd3dDevice, filePath.c_str(), &m_pTexture);
}
