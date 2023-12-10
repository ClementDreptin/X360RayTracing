#pragma once

#include "UI/Text.h"

class Console
{
public:
    Console();

    Console(size_t messageCount);

    void Log(const std::wstring &message);

    void Render(float x, float y);

private:
    size_t m_MessageCount;
    std::deque<std::wstring> m_Messages;
    Text m_TextBox;
};

extern Console g_Console;
