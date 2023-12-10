#include "pch.h"

#include "Core/App.h"
#include "Renderer/Globals.h"

void __cdecl main()
{
    App app;
    app.m_d3dpp.BackBufferWidth = DISPLAY_WIDTH;
    app.m_d3dpp.BackBufferHeight = DISPLAY_HEIGHT;

    // Make sure display gamma is correct
    app.m_d3dpp.BackBufferFormat = static_cast<D3DFORMAT>(MAKESRGBFMT(D3DFMT_A8R8G8B8));
    app.m_d3dpp.FrontBufferFormat = static_cast<D3DFORMAT>(MAKESRGBFMT(D3DFMT_LE_X8R8G8B8));

    app.Run();
}
