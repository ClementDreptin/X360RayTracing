#pragma once

class Texture
{
public:
    Texture();

    inline D3DTexture *operator&() const { return m_pTexture; }

    HRESULT Init(const std::string &filePath);

private:
    D3DTexture *m_pTexture;
};
