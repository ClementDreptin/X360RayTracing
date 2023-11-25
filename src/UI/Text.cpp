#include "pch.h"
#include "UI/Text.h"

#include "UI/Font.h"

HRESULT Text::Render(const Props &props)
{
    HRESULT hr = S_OK;

    float fontScale = props.FontScale != 0.0f ? props.FontScale : 1.0f;
    g_Font.SetScaleFactors(fontScale, fontScale);

    // Render the text
    g_Font.DrawText(props.X, props.Y, props.Color, props.Text.c_str());
    g_Font.SetScaleFactors(1.0f, 1.0f);

    return hr;
}
