#pragma once

#include <AtgApp.h>
#include <AtgResource.h>
#include <AtgUtil.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/PixelShader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexShader.h"
#include "UI/Text.h"

class App : public ATG::Application
{
public:
    App();

    virtual HRESULT Initialize();

    virtual HRESULT Update();

    virtual HRESULT Render();

private:
    ATG::Timer m_Timer;
    Text m_FrameRateText;

    ATG::PackedResource m_Textures;

    D3DTexture *m_pBackgroundTexture;
    VertexBuffer m_BackgroundVertexBuffer;
    IndexBuffer m_BackgroundIndexBuffer;
    VertexShader m_BackgroundVertexShader;
    PixelShader m_BackgroundPixelShader;

    HRESULT InitBackground();

    void RenderBackground();

    void RenderFrameRateText();
};
