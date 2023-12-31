#include "pch.h"
#include "UI/Text.h"

#include "UI/Font.h"

void Text::Render(const Props &props)
{
    float fontScale = props.FontScale != 0.0f ? props.FontScale : 1.0f;
    g_Font.SetScaleFactors(fontScale, fontScale);

    g_Font.DrawText(props.X, props.Y, props.Color, props.Text.c_str());
    g_Font.SetScaleFactors(1.0f, 1.0f);
}
