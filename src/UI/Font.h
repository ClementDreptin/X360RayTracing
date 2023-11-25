#pragma once

#include <AtgFont.h>

class Font : public ATG::Font
{
public:
    Font() {}

    virtual ~Font() {}

    float GetTextWidth(const std::wstring &text);

    float GetTextHeight(const std::wstring &text);

    void GetTextDimensions(const std::wstring &text, float *pWidth = nullptr, float *pHeight = nullptr) const;
};

extern Font g_Font;
