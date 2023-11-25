#include "pch.h"
#include "UI/Console.h"

#include "UI/Font.h"

Console::Console()
    : m_NumberOfMessages(5)
{
}

Console::Console(size_t numberOfMessages)
    : m_NumberOfMessages(numberOfMessages)
{
}

void Console::Log(const std::wstring &message)
{
    m_Messages.push_back(message);

    // Remove the last element when a new one is push to keep the size constant
    if (m_Messages.size() > m_NumberOfMessages)
        m_Messages.pop_front();
}

HRESULT Console::Render(float x, float y)
{
    Text::Props props = { 0 };
    props.X = x;
    props.Y = y;
    props.Text = L"";
    props.Color = D3DCOLOR_XRGB(255, 255, 255);

    // Generate the text from the messages
    for (size_t i = 0; i < m_NumberOfMessages; i++)
    {
        if (i < m_Messages.size())
            props.Text += m_Messages[i];

        // Add a newline character between the messages (so not after the last one)
        if (i != m_Messages.size() - 1)
            props.Text += L'\n';
    }

    return m_TextBox.Render(props);
}
