#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/VertexBuffer.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render(const Scene &scene, const Camera &camera);

private:
    VertexBuffer m_VertexBuffer;
    ConstantBuffer<Scene> m_ConstantBuffer;

    static bool s_ShadersInitialized;
    static D3DVertexShader *s_pVertexShader;
    static D3DPixelShader *s_pPixelShader;

    static HRESULT InitShaders();
};
