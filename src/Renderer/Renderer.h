#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Image.h"
#include "Renderer/Ray.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render(const Scene &scene, const Camera &camera);

private:
    Image m_Image;

    XMCOLOR TraceRay(const Scene &scene, const Ray &ray);
};
