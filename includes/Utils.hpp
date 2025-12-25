#pragma once

#include <Types.hpp>

#define LINE_BREAK_CMD "\n"

template<typename T>
void VecPrint(const Vec<T>& v, const String& title = "Vector", const String& sep = ", ")
{
    std::cout << LINE_BREAK_CMD;
    std::cout << title << LINE_BREAK_CMD;
    for (const auto& item: v)
    {
        std::cout << item << sep;
    }
    std::cout << LINE_BREAK_CMD;
}