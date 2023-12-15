#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Image.h"
#include "Renderer/Ray.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render(const Camera &camera);

private:
    Image m_Image;

    XMCOLOR TraceRay(const Ray &ray);
};