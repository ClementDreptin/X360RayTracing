#pragma once

class PixelShader
{
public:
    PixelShader();

    inline D3DPixelShader *operator&() const { return m_pShader; }

    HRESULT Init(const std::string &filePath);

private:
    D3DPixelShader *m_pShader;
};
