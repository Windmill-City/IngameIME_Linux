#pragma once
#include <string>

std::string format(const char* pMessage, ...)
{
    va_list args = NULL;
    va_start(args, pMessage);

    va_end(args);

    auto result = std::string(pBufMsg);

    return result;
}

std::wstring format(const wchar_t* pMessage, ...)
{
    va_list args = NULL;
    va_start(args, pMessage);

    auto result = std::wstring(pBufMsg);

    return result;
}