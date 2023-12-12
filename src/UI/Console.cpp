#include "pch.h"
#include "UI/Console.h"

#include "UI/Font.h"

Console::Console()
    : m_MessageCount(5)
{
}

Console::Console(size_t messageCount)
    : m_MessageCount(messageCount)
{
}

void Console::Log(const std::wstring &message)
{
    m_Messages.push_back(message);

    // Rotate the message to have at most m_MessageCount on the screen
    if (m_Messages.size() > m_MessageCount)
        m_Messages.pop_front();
}

void Console::Render(float x, float y)
{
    Text::Props props;
    props.X = x;
    props.Y = y;
    props.Text = L"";
    props.Color = XMCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    for (size_t i = 0; i < m_MessageCount; i++)
    {
        if (i < m_Messages.size())
            props.Text += m_Messages[i];

        if (i != m_Messages.size() - 1)
            props.Text += L'\n';
    }

    m_TextBox.Render(props);
}
