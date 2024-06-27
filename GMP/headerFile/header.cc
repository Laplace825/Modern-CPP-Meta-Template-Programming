/***
 * @author: "laplace"
 * @date: 2024-06-27T22:06:42
 * @lastmod: 2024-06-27T22:06:48
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/headerFile/header.cc
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 ***/

// #include "include/header.hpp"

#include <iostream>

namespace lap {
template < typename T >
void foo(T)
{
    std::cout << "foo(T)" << std::endl;
}
} // namespace lap