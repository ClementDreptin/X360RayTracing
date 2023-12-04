#pragma once

#include "UI/Text.h"

class Console
{
public:
    Console();

    Console(size_t numberOfLines);

    void Log(const std::wstring &message);

    void Render(uint32_t x, uint32_t y);

private:
    size_t m_NumberOfMessages;
    std::deque<std::wstring> m_Messages;
    Text m_TextBox;
};

extern Console g_Console;
