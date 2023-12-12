#pragma once

class Text
{
public:
    struct Props
    {
        Props()
            : X(0), Y(0), FontScale(0.0f), Color(XMCOLOR(0.0f, 0.0f, 0.0f, 0.0f)) {}

        float X;
        float Y;
        std::wstring Text;
        float FontScale;
        XMCOLOR Color;
    };

    Text() {}

    void Render(const Props &props);
};
