// basic interface defined for the random number generator

#pragma once

#include <includes/Types.hpp>

#define DEFAULT_SEED 143

template<class T = uint>
class IRandomNumberGenerator 
{
    public:

    protected:
    T m_initalSeed { DEFAULT_SEED };
    T m_lastVal {};
};