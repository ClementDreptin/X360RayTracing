#pragma once

class Text
{
public:
    struct Props
    {
        Props()
            : X(0), Y(0), FontScale(0.0f), Color(0) {}

        uint32_t X;
        uint32_t Y;
        std::wstring Text;
        float FontScale;
        D3DCOLOR Color;
    };

    Text() {}

    void Render(const Props &props);
};
