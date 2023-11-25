#pragma once

class Text
{
public:
    struct Props
    {
        float X;
        float Y;
        std::wstring Text;
        float FontScale;
        D3DCOLOR Color;
    };

    Text() {}

    HRESULT Render(const Props &props);
};
