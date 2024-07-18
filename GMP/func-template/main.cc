/*
 * @Author: Laplace825
 * @Date: 2024-06-13 09:25:59
 * @LastEditTime: 2024-06-13 20:07:53
 * @LastEditors: Laplace825
 * @FilePath: \EffectiveModernCPP\GMP\func-template\main.cc
 * @Description:
 * Copyright (c) 2024 by Laplace825, All Rights Reserved.
 */
#include <array>
#include <cstring>
#include <print>
#include <type_traits>

template < typename T1, typename T2 >
std::common_type_t< T1, T2 > getMax(T1 a, T2 b)
{
    // 若能转化,类型会发生向上隐式转换
    return b < a ? a : b;
}

template < typename T1, typename T2 >
auto getMax2(T1 a, T2 b) -> decltype(b < a ? a : b)
// eq to  -> decltype(true ? a : b)
{
    /**
     *  @note: decltype() 会推导出表达式的类型
     *  其返回值为左右表达式的类型,不需要额外多一次比较
     */
    return b < a ? a : b;
}

// 避免返回类型为引用
template < typename T1, typename T2 >
auto getMax3(T1 a, T2 b) -> std::decay_t< decltype(true ? a : b) >
{
    return b < a ? a : b;
}

template < typename T1, typename T2,
           typename RT = std::decay_t< decltype(true ? T1() : T2()) > >
// T1() 为 T1 类型的默认构造
RT getMax4(T1 a, T2 b)
{
    return b < a ? a : b;
}

template < typename T >
T getMaxSameType(T a, T b)
{
    // 模版禁止自动类型转化(隐式类型转换)
    std::println("T");
    return b < a ? a : b;
}

int getMaxSameType(int a, int b)
{
    std::println("int");
    return b < a ? a : b;
}

template < typename T >
T const &getMaxRefer(T const &a, T const &b)
{
    return b < a ? a : b;
}

char const *getMaxRefer(char const *a, char const *b)
{
    return std::strcmp(b, a) < 0 ? a : b;
}

template < typename T >
T const &getMaxRefer(T const &a, T const &b, T const &c)
{
    /**
     *  @note:
     * 这里是因为getMaxRefer(a,b)会返回一个临时变量,然后这个临时变量又继续getMaxRefer(tmp,c)
     * 并且可能被继续返回,而此时就返回了一个对于临时变量的引用
     */
    return getMaxRefer(getMaxRefer(a, b), c);
}

template < typename T1, typename T2 >
constexpr auto getMaxConstexpr(T1 a, T2 b)
{
    return b < a ? a : b;
}

auto main() -> int
{
    using std::println;
    auto res  = getMax(1, 2.2);
    auto res2 = getMax2(1, 0.1);
    auto res3 = getMax3(1, 0.1);
    auto res4 = getMax4(1, 0.1);
    auto x    = int();

    getMaxSameType('a', 9);
    getMaxSameType<>(1, 10); // 调用模版函数,解析为模版调用
    // getMaxSameType<>('a',9); // 无法推导类型
    getMaxSameType('a', static_cast< char >(9)); // 此时类型为 char

    char const *s1 = "zrederic";
    char const *s2 = "anica";
    char const *s3 = "lucas";
    // auto x2 = getMaxRefer(s1, s2, s3);

    constexpr auto x3 = getMaxConstexpr(1000u, sizeof(char));
    println("max of 1000u and sizeof char : {}", x3);
    std::array< int, x3 > arr = {1, 2, 3, 4, 5};

    return 0;
}
