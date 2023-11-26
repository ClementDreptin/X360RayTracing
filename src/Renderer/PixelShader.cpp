#include "pch.h"
#include "Renderer/PixelShader.h"

#include <AtgUtil.h>

PixelShader::PixelShader()
    : m_pShader(nullptr)
{
}

HRESULT PixelShader::Init(const std::string &filePath)
{
    HRESULT hr = ATG::LoadPixelShader(filePath.c_str(), &m_pShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load the pixel shader at %s", filePath.c_str());
        return hr;
    }

    return hr;
}
