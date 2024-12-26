#pragma once

namespace Log
{

void Print(const char *format, ...);

void Print(const std::string &message);

void Print(const wchar_t *format, ...);

void Print(const std::wstring &message);

}

#ifndef NDEBUG
    #define DebugPrint(format, ...) Log::Print(format, __VA_ARGS__)
#else
    #define DebugPrint(format, ...)
#endif
