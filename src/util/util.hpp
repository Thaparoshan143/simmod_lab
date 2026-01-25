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

/*
Check if the element exists on pointer array,
on success returns the index else returns -1
*/
template<typename T>
static int ArrContains(const T *arr, const uint count, const T& c)
{
    for (uint i=0;i<count;++i)
    {
        const auto& item = *(arr + i);
        if (item == c) 
            return i;
    }

    return -1;
}

/*
Check if the element exists on vector,
on success returns the index else returns -1
*/
template<typename T>
static int VecContains(const Vector<T>& vec, const T& c)
{
    for (uint i=0;i<vec.size();++i)
    {
        const auto& item = vec[i];
        if (item == c) 
            return i;
    }

    return -1;
}

/*
Multiply the Vector by single scalar (one scalar multiple to all element in vector)
# Warning: original vector is modified with result.
*/
// template<typename T, typename U>
// static void VecScalMul(Vector<T>& vec, const U& sca)
// {
//     for (auto& item: vec)
//     {
//         item *= sca;
//     }
// }

/*
Multiply the Vector by single scalar (one scalar multiple to all element in vector)
returns new vector with result
*/
template<typename T, typename U>
static Vector<T> VecScalMul(const Vector<T>& vec, const U& sca)
{
    Vector<T> res {};
    res.reserve(vec.size());
    for (const auto& item: vec)
    {
        res.push_back(item * sca);
    }
    return res;
}

/*
Multiply the Vector by complex scalar (one complex valued scalar multiple to all element in Complex vector)
# Warning: original vector is modified with result.
*/
template<typename T>
static void VecScalMul(Vector<Complex<T>> vec, const Complex<T>& sca)
{
    for (auto& item: vec)
    {
        item *= sca;
    }
}

/*
Callback for each vector element passed as arg for some operation
# Warning: original vector is modified with result.
*/
// template<typename VecType, typename Op>
// static void VecOpCb(VecType& vec, Op cb)
// {
//     for (auto& item: vec)
//     {
//         item = cb(item);
//     }
// }

/*
Callback for each vector element passed as arg for some operation (lambda)
return is new vector with result.. 
*/
template<typename T, typename Op>
static Vector<T> VecOpCb(const Vector<T>& vec, Op cb)
{
    Vector<T> res {};
    res.reserve(vec.size());

    for (const auto& item: vec)
    {
        res.push_back((cb(item)));
    }
    return res;
}

/*
Callback for each vector element passed as arg for some operation (lambda)
return is new vector with result.. stored in complex form
*/
template<typename T, typename Op>
static Vector<Complex<T>> VecOpCbComplex(const Vector<T>& vec, Op cb)
{
    Vector<Complex<T>> res {};
    res.reserve(vec.size());

    for (const auto& item: vec)
    {
        res.push_back((cb(item)));
    }
    return res;
}

template<typename T>
static Vector<Complex<T>> RealToComplex(const Vector<T>& vec)
{
    Vector<Complex<T>> res {};
    res.reserve(vec.size());

    for (const auto& item: vec)
    {
        res.push_back(Complex<T>(item, 0));
    }
    return res;
}

/*
Callback for dual vector where each element of both vector (same index) passed as arg for some operation in callback (lambda)
return the result 
*/
template<typename T, typename Op>
static Vector<T> DualVecOpCb(const Vector<T>& vec1, const Vector<T>& vec2, Op cb)
{
    if (vec1.size() != vec2.size())
    {
        std::cerr << "Vector size must be same!" << std::endl;
        exit(-1);
    }

    Vector<T> res {};
    res.reserve(vec1.size());

    for (uint i=0;i<vec1.size();++i)
    {
        res.push_back((cb(vec1[i], vec2[i])));
    }

    return res;
}

/*
function for calculation of Complex number magnitudes
returns new vector with magnitude
*/
template<typename T>
static Vector<T> GetComplexVecMag(const Vector<Complex<T>>& vec)
{
    Vector<T> res {};
    res.reserve(vec.size());

    for (const auto& item: vec)
    {
        res.push_back(std::abs(item));
    }

    return res;
}

#define DEGREE_MULTIPLE 180
#define PI 3.1415
/*
function for calculation of Complex number Phase
returns new vector with magnitude
*/
template<typename T>
static Vector<T> GetComplexVecPhase(const Vector<Complex<T>>& vec, const bool isDegree = false)
{
    Vector<T> res {};
    res.reserve(vec.size());

    T theta {};

    for (const Complex<T>& item: vec)
    {
        theta = atan2(item.imag(), item.real());
        res.push_back(isDegree ? theta * DEGREE_MULTIPLE / PI : theta);
    }

    return res;
}

template<typename T>
static T GetVectorMaxValue(const Vector<T>& vec)
{
    T maxVal;
    for (const auto& item: vec)
    {
        if (item > maxVal)
        {
            maxVal = item;
        }
    }
    return maxVal;
}

template<typename T>
static T GetVectorMinValue(const Vector<T>& vec)
{
    T minVal { T(~1) };
    for (const auto& item: vec)
    {
        if (item < minVal)
        {
            minVal = item;
        }
    }
    return minVal;
}

/*
returns new vector with normalized element in range of 0 to 1
*/
template<typename T, typename U>
static Vector<T> GetVectorNormalize(const Vector<U>& vec)
{
    Vector<T> res {};
    res.reserve(vec.size());

    auto vecMax = GetVectorMaxValue(vec);
    auto vecMin = GetVectorMinValue(vec);

    // std::cout << vecMax << " .. " << vecMin << std::endl;
    // std::cout << "*********" << std::endl;
    for (const auto& item: vec)
    {
        res.push_back((float(item) - vecMin) / (vecMax - vecMin));
    }

    return res;
}

/*
returns new vector with normalized element in range of 0 to 1 given n
*/
template<typename T, typename U>
static Vector<T> GetVectorNormalize(const Vector<U>& vec, const T val)
{
    Vector<T> res {};
    res.reserve(vec.size());

    auto vecMax = GetVectorMaxValue(vec);
    auto vecMin = GetVectorMinValue(vec);

    // std::cout << vecMax << " .. " << vecMin << std::endl;
    // std::cout << "*********" << std::endl;
    for (const auto& item: vec)
    {
        res.push_back((float(item) / val));
    }

    return res;
}
