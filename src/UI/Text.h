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

    void Render(const Props &props);
};
