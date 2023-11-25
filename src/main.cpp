#include "pch.h"

#include "Core/App.h"

extern float g_DisplayWidth;
extern float g_DisplayHeight;

void __cdecl main()
{
    App app;
    app.m_d3dpp.BackBufferWidth = static_cast<uint32_t>(g_DisplayWidth);
    app.m_d3dpp.BackBufferHeight = static_cast<uint32_t>(g_DisplayHeight);

    // Make sure display gamma is correct.
    app.m_d3dpp.BackBufferFormat = static_cast<D3DFORMAT>(MAKESRGBFMT(D3DFMT_A8R8G8B8));
    app.m_d3dpp.FrontBufferFormat = static_cast<D3DFORMAT>(MAKESRGBFMT(D3DFMT_LE_X8R8G8B8));

    app.Run();
}
