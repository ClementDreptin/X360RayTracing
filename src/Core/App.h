#pragma once

#include <AtgApp.h>
#include <AtgUtil.h>

#include "Renderer/Image.h"
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
    Image m_Image;

    D3DCOLOR PerPixel(const XMVECTOR &coord);

    void RenderImage();

    void RenderFrameRateText();
};
