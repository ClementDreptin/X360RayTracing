#pragma once

#include "Renderer/Image.h"

class Renderer
{
public:
    Renderer();

    HRESULT Init();

    void Render();

private:
    Image m_Image;

    XMCOLOR PerPixel(const XMVECTOR &coord);
};
