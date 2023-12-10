#include "pch.h"
#include "Core/Log.h"

#include "UI/Console.h"

void Log::Info(const char *message, ...)
{
    va_list args;
    va_start(args, message);

    std::string fullMessage = "Info: ";
    fullMessage += message;
    Print(fullMessage.c_str(), args);

    va_end(args);
}

void Log::Error(const char *message, ...)
{
    va_list args;
    va_start(args, message);

    std::string fullMessage = "Error: ";
    fullMessage += message;
    Print(fullMessage.c_str(), args);

    va_end(args);
}

void Log::Print(const char *format, const va_list args)
{
    char buffer[2048] = {};
    vsnprintf_s(buffer, _TRUNCATE, format, args);

    wchar_t wideBuffer[2048] = {};
    mbstowcs_s(nullptr, wideBuffer, buffer, _TRUNCATE);

    g_Console.Log(wideBuffer);
}
