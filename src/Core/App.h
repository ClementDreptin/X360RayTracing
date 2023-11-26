#pragma once

#include <AtgApp.h>
#include <AtgResource.h>
#include <AtgUtil.h>

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
    D3DVertexBuffer *m_pBackgroundVertexBuffer;
    D3DVertexDeclaration *m_pBackgroundVertexDeclaration;
    D3DIndexBuffer *m_pBackgroundIndexBuffer;
    D3DVertexShader *m_pBackgroundVertexShader;
    D3DPixelShader *m_pBackgroundPixelShader;

    HRESULT InitBackground();

    HRESULT RenderBackground();

    HRESULT RenderFrameRateText();
};
