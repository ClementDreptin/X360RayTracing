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
        Log::Error("Couldn't create the background vertex shader");
        return hr;
    }

    return hr;
}
