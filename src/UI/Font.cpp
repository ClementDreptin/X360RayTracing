#include "pch.h"
#include "UI/Font.h"

float Font::GetTextWidth(const std::wstring &text)
{
    float width = 0.0f;
    GetTextDimensions(text, &width);

    return width;
}

float Font::GetTextHeight(const std::wstring &text)
{
    float height = 0.0f;
    GetTextDimensions(text, nullptr, &height);

    return height;
}

void Font::GetTextDimensions(const std::wstring &text, float *pWidth, float *pHeight) const
{
    float biggestWidth = 0.0f;
    float currentLineWidth = 0.0f;
    float height = m_fFontHeight;
    const wchar_t *str = text.c_str();

    wchar_t letter = L'\0';
    while ((letter = *str) != 0)
    {
        ++str;

        // Handle newline characters
        if (letter == L'\n')
        {
            // Reset the current line width and increase the height by the font height
            currentLineWidth = 0.0f;
            height += m_fFontYAdvance;

            continue;
        }

        // Just ignore carriage return characters
        if (letter == L'\r')
            continue;

        // Clamp the character if it's out of bounds and get its corresponding glyph index
        if (letter > m_cMaxGlyph)
            letter = L'\0';
        else
            letter = m_TranslatorTable[letter];

        // Get the glyph from the glyph index and increment the current line width by the glyph width
        const ATG::GLYPH_ATTR *pGlyph = &m_Glyphs[letter];
        currentLineWidth += pGlyph->wOffset + pGlyph->wAdvance;

        if (currentLineWidth > biggestWidth)
            biggestWidth = currentLineWidth;
    }

    biggestWidth *= m_fXScaleFactor;
    height *= m_fYScaleFactor;

    if (pWidth != nullptr)
        *pWidth = biggestWidth;

    if (pHeight != nullptr)
        *pHeight = height;
}
