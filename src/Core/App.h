#pragma once

#include <AtgApp.h>
#include <AtgUtil.h>

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"
#include "UI/Text.h"

class App : public ATG::Application
{
public:
    App();

    virtual HRESULT Initialize();

    virtual HRESULT Update();

    virtual HRESULT Render();

private:
    Camera m_Camera;
    Renderer m_Renderer;
    Scene m_Scene;
    ATG::Timer m_Timer;
    Text m_FrameRateText;

    void RenderFrameRateText();
};
