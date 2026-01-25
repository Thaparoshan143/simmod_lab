#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <unordered_map>

template<class T>
using Vector = std::vector<T>;

using uint = unsigned int;

using String = std::string;

template<class T = double>
using Complex = std::complex<T>;

template<class K, typename V>
using Map = std::unordered_map<K, V>;