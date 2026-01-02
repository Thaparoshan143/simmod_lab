#pragma once

#include <iostream>
#include <includes/Types.hpp>

// #define END_LINE_CMD std::cout
#define END_LINE_CMD "\n"
#define DEFAULT_SEP_SYMBOL "-"
#define DEFAULT_SEP_COUNT 60


static inline void Seperator(const String& sep = DEFAULT_SEP_SYMBOL, const uint count = DEFAULT_SEP_COUNT)
{
    for (uint i=0;i<count;++i)
    {
        std::cout << sep;
    }
    std::cout << END_LINE_CMD;
}

template<typename T>
static void PrintVec(const Vector<T>& vec, const String& title, const String& vsep = ", ")
{
    Seperator();
    std::cout << title << END_LINE_CMD;
    for (const auto& item: vec)
    {
        std::cout << item << vsep;
    }
    std::cout << END_LINE_CMD;
    Seperator();
}