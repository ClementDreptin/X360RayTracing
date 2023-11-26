#pragma once

class VertexShader
{
public:
    VertexShader();

    inline D3DVertexShader *operator&() const { return m_pShader; }

    HRESULT Init(const std::string &filePath);

private:
    D3DVertexShader *m_pShader;
};
