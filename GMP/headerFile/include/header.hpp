/***
 * @author: "laplace"
 * @date: 2024-06-27T22:06:27
 * @lastmod: 2024-06-27T22:06:34
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/headerFile/header.hpp
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 ***/
#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <iostream>

namespace lap {

// Bad
// Linker error 最好还是把模版定义放在头文件中
// template < typename T >
// void foo(T);

template < typename T >
void foo(T)
{
    std::cout << "foo(T)" << std::endl;
}

} // namespace lap

#endif // _HEADER_HPP_