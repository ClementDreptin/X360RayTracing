#pragma once

#include <AtgResource.h>

class Texture
{
public:
    Texture();

    inline D3DTexture *operator&() const { return m_pTexture; }

    HRESULT Init(const std::string &name);

private:
    static ATG::PackedResource s_Textures;
    D3DTexture *m_pTexture;
};
