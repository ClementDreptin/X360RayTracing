#pragma once

#include <AtgApp.h>
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

    HRESULT RenderFrameRateText();
};
