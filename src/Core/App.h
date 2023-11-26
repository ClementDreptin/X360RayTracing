#pragma once

#include <AtgApp.h>
#include <AtgResource.h>
#include <AtgUtil.h>

#include "Renderer/VertexBuffer.h"
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
    VertexBuffer m_VertexBuffer;
    D3DIndexBuffer *m_pBackgroundIndexBuffer;
    D3DVertexShader *m_pBackgroundVertexShader;
    D3DPixelShader *m_pBackgroundPixelShader;

    HRESULT InitBackground();

    HRESULT RenderBackground();

    HRESULT RenderFrameRateText();
};
