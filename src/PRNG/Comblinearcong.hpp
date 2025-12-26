// Implementation of Combined Linear Congruential Method for generating Random Number

#pragma once

#include <Types.hpp>
#include <PRNG/Linearcong.hpp>

#define DEFAULT_COMB_LIN_COUNT 3 // i.e will use three linear cong. generator

/**
    Brief details
    Sequence is produced between 0 to m-1 (range given) using recursive relation of multiple linear cong. generator as follows
    X(i) = summation from j=1 to k [(-1)^(j-1) * X(i, j)] mod m
    where, k defined the number of linear cong. generator used.

    - X(i, j) are the ith output from different multiplication congruential (linear)

    Normally, random number is calculated as:
    R(i)    = X(i) / m(j), if X(i) > 0
            = (m(j) - 1)/m(j), if X(i) = 0
*/
class CombLinearCongRNGen: public IRandomNumberGenerator<uint>
{
    public:
    // CombLinearCongRNGen() = delete;

    void Init(const uint m, const uint lgenCount = DEFAULT_COMB_LIN_COUNT, const uint seed = DEFAULT_SEED, const uint a = DEFAULT_MULTIPLIER, const uint c = DEFAULT_ADDITIVE)
    {
        m_initalSeed = seed; // kept only for any further usage..
        m_lastVal = seed;
        m_multiplier = a;
        m_additive = c;
        m_max = m;
        initAllLinearGen(lgenCount);
    }

    // returns the next value in sequence of random value generation
    uint Next()
    {
        for (uint i=0;i<m_linearGen.size();++i)
        {
            auto& gen = m_linearGen[i];

            m_lastVal += gen.Next() * static_cast<uint>(pow(-1, i));
        }

        m_lastVal = m_lastVal % m_max;
        return m_lastVal;
    }

    Vec<uint> NextFew(const uint count)
    {
        Vec<uint> seq {};
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
        // also reset all linear geneartor..
        for (auto& lg: m_linearGen)
        {
            lg.Reset();
        }
    }

    private:
    Vec<LinearCongRNGen> m_linearGen {};
    uint m_multiplier {};
    uint m_additive {};
    uint m_max {};

    void initAllLinearGen(const uint lc)
    {
        uint _max { m_max }, _iniSeed { m_initalSeed }, _mul { m_multiplier }, _add { m_additive };

        // this is custom implementation.. creating the generator for init value generation of other linear generator..
        LinearCongRNGen valGen {};
        valGen.Init(_max, _iniSeed, _mul, _add);

        for (uint i=0;i<lc;++i)
        {
            // can be as per wish.. this is just illustartions..
            _max = valGen.Next();
            _iniSeed = i + 1; 
            _mul = valGen.Next();
            _add = valGen.Next();

            // std::cout << "Init values are: " << std::endl;
            // std::cout << "Max: " << _max << std::endl;
            // std::cout << "Initial Seed: " << _iniSeed << std::endl;
            // std::cout << "Mul: " << _mul << std::endl;
            // std::cout << "Add: " << _add << std::endl;

            LinearCongRNGen temp {};
            temp.Init(_max, _iniSeed, _mul, _add);
            m_linearGen.push_back(temp);
        }
    }
};