#pragma once
#include <memory>
#include <stdarg.h>
#include <string>

std::string format(const char* pMessage, ...)
{
    va_list args;
    va_start(args, pMessage);

    char buf[256];
    auto len = vsnprintf(buf, 256, pMessage, args);

    va_end(args);

    if (len < 0) throw std::runtime_error("Failed to format message!");
  
    auto result = std::string(buf, len);

    return result;
}

std::wstring format(const wchar_t* pMessage, ...)
{
    va_list args;
    va_start(args, pMessage);

    wchar_t buf[256];
    auto    len = vswprintf(buf, 256, pMessage, args);

    va_end(args);

    if (len < 0) throw std::runtime_error("Failed to format message!");

    auto result = std::wstring(buf, len);

    return result;
}