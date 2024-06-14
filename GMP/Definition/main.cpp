/*
 * @Author: laplace825
 * @Date: 2024-04-28 22:01:50
 * @LastEditors: Laplace825
 * @LastEditTime: 2024-06-13 12:52:10
 * @FilePath: \EffectiveModernCPP\GMP\Definition\main.cpp
 * @Description:
 *
 * Copyright (c) 2024 by laplace825, All Rights Reserved.
 */
#include <format>
#include <iostream>
#include <string>

using std::format;

template <typename Type>
Type add(Type a, Type b)
{
    // NOTE: 一次定义多次调用
    std::cout << format("add({},{}) ", a, b);
    return a + b;
}

template <typename T>
struct GetPtr
{
    using type = T *;
};

template <typename T>
using GetPtr_t = GetPtr<T>::type;

signed main()
{
    std::cout << add(1, 2) << '\n';
    std::cout << add(1.1, 2.1) << '\n';
    std::cout << add<>(2, 5) << '\n';
    std::cout << add(std::string("Hello "), std::string("world!")) << '\n';

    int a = 1;
    double d = 2.2;
    // add(a, d);  // 报错 Type 无法推导

    add<double>(a, d); // NOTE: 出现类型转换

    // add(); // 不能不实例化且不传入

    // NOTE: 没有参数无法推导
    // add<>();
    // add<int>();

    // NOTE: 实际上定义了一个 int*
    GetPtr_t<int> ptr;
}
