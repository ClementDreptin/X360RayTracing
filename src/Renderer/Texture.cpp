#include "pch.h"
#include "Renderer/Texture.h"

ATG::PackedResource Texture::s_Textures;

Texture::Texture()
    : m_pTexture(nullptr)
{
}

HRESULT Texture::Init(const std::string &name)
{
    HRESULT hr = S_OK;

    if (!s_Textures.Initialized())
    {
        hr = s_Textures.Create("game:\\Media\\Textures\\Textures.xpr");
        if (FAILED(hr))
        {
            Log::Error("Couldn't create the textures");
            return hr;
        }
    }

    m_pTexture = s_Textures.GetTexture(name.c_str());
    if (m_pTexture == nullptr)
    {
        Log::Error("Couldn't get the texture named %s", name.c_str());
        return E_FAIL;
    }

    return hr;
}
