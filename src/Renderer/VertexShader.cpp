#include "pch.h"
#include "Renderer/VertexShader.h"

#include <AtgUtil.h>

VertexShader::VertexShader()
    : m_pShader(nullptr)
{
}

HRESULT VertexShader::Init(const std::string &filePath)
{
    HRESULT hr = ATG::LoadVertexShader(filePath.c_str(), &m_pShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load the vertex shader at %s", filePath.c_str());
        return hr;
    }

    return hr;
}
