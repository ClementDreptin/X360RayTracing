#pragma once

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
    Scene m_Scene;
    Camera m_Camera;
    Renderer m_Renderer;
    ATG::Timer m_Timer;
    Text m_FrameRateText;

    void RenderFrameRateText();
};
