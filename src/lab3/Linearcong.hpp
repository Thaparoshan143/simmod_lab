// Implementation of Linear Congruential Method for generating Random Number

#pragma once

#include <includes/Types.hpp>
#include <lab3/IRandom.hpp>

#define DEFAULT_MULTIPLIER 8
#define DEFAULT_ADDITIVE 1

/**
    Brief details
    Sequence is produced between 0 to m-1 (range given) using recursive relation as follows
    X(i+1) = (aX(i) + c) mode m, for i=0,1,2,..

    - The initial integer X(0) is known as seed
    - a is multiplier
    - c is increment
    - m is the modulus (defined by range)

    #### Limitation: only non-negative integer random number between 0 to m-1, also repeats & breaks for different inital cond. like when c=0 & seed=0 (i.e only 0 sequence).. likewise same sequence (i.e only 42..)

    @cases: 
    1. a = 1 then additive, 
    2. c = 0 multiplicative,
    3. a > 1 & c > 0 mixed type

*/
class LinearCongRNGen: public IRandomNumberGenerator<uint>
{
    public:
    // LinearCongRNGen() = delete;

    void Init(const uint m, const uint seed = DEFAULT_SEED, const uint a = DEFAULT_MULTIPLIER, const uint c = DEFAULT_ADDITIVE)
    {
        m_initalSeed = seed; // kept only for any further usage..
        m_lastVal = seed;
        m_multiplier = a;
        m_additive = c;
        m_max = m;
    }

    // returns the next value in sequence of random value generation
    uint Next()
    {
        m_lastVal = (m_multiplier * m_lastVal + m_additive) % m_max;
        return m_lastVal;
    }

    Vector<uint> NextFew(const uint count)
    {
        Vector<uint> seq {};
        for (uint i=0;i<count;++i)
        {
            seq.push_back(Next());
        }
        return seq;
    }

    // resets the sequence start from seed value.. (i.e, in initial seed)
    void Reset()
    {
        m_lastVal = m_initalSeed;
    }

    private:
    uint m_multiplier {};
    uint m_additive {};
    uint m_max {};
};