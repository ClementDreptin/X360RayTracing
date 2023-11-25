#include "pch.h"
#include "Core/Log.h"

#include "UI/Console.h"

void Log::Info(const char *message, ...)
{
    // Get the variadic arguments
    va_list args;
    va_start(args, message);

    // Print
    std::string fullMessage = "Info: ";
    fullMessage += message;
    Print(fullMessage.c_str(), args);

    // Free the variadic arguments
    va_end(args);
}

void Log::Error(const char *message, ...)
{
    // Get the variadic arguments
    va_list args;
    va_start(args, message);

    // Print
    std::string fullMessage = "Error: ";
    fullMessage += message;
    Print(fullMessage.c_str(), args);

    // Free the variadic arguments
    va_end(args);
}

void Log::Print(const char *format, const va_list args)
{
    // Build the string with the format
    char buffer[2048] = { 0 };
    vsnprintf_s(buffer, _TRUNCATE, format, args);

    // Convert the narrow string to a wide string
    wchar_t wideBuffer[2048] = { 0 };
    mbstowcs_s(nullptr, wideBuffer, buffer, _TRUNCATE);

    // Print
    g_Console.Log(wideBuffer);
}
