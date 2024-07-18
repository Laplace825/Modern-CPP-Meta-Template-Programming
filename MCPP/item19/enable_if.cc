/*** 
 * @author: "laplace"
 * @date: 2024-06-13T09:24:00
 * @lastmod: 2024-06-26T15:18:59
 * @description:  
 * @filePath: /EffectiveModernCPP/MCPP/item19/enable_if.cpp
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
**/
#include <functional>
#include <iostream>
#include <type_traits>

template < typename T >
struct Func1
{
    using type = T;
};

template < size_t N >
struct Func2
{
    int data[N];
};

template < typename T >
typename std::enable_if< std::is_same_v< int, T >, T >::type func(T x)
{
    std::cout << "func(T x)" << std::endl;
    return x;
}

int main()
{
    Func1< int >::type a = 10;
    Func2< 10 > b;
    // func('1');
    return 0;
}
