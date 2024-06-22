/***
 * @author: "laplace"
 * @date: 2024-06-20T23:29:31
 * @lastmod: 2024-06-20T23:29:39
 * @description:
 * @f
 namespace lap{ilePath: /EffectiveModernCPP/GMP/forward/main.cc
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 **/
#include <print>
#include <tuple>
#include <iostream>
#include <utility>
#include <vector>

namespace lap {
template <typename X>
void g(X &)
{
    std::cout << "g() for variable\n";
}

template <typename X>
void g(X const &)
{
    std::cout << "g() for constant\n";
}

template <typename X>
void g(X &&)
{
    std::cout << "g() for movable object\n";
}

// let f() forward argument val to g():
// @note: 无法针对上面的f()进行重载,而且调用f()时,会根据传入的参数类型进行选择,
// 常发生值类型退化
template <typename X>
void f(X val)
{
    g(val);
};

template <typename T>
void fforward(T &&val)
{
    g(std::forward<T>(val));
}

template <typename T, size_t... _Vals>
struct integer_sequence
{
    using type = T;
    static constexpr std::size_t value_size = sizeof...(_Vals);
};

template <typename T, size_t N, T... _Vals>
struct _make_integer_sequence
    : _make_integer_sequence<T, N - 1, N - 1, _Vals...>
{
    /**
     * @brief: 递归生成序列
     * 每次递归都会生成一个新的继承类,直到N为0
     **/
};

template <typename T, T... _Vals>
struct _make_integer_sequence<T, 0, _Vals...> : integer_sequence<T, _Vals...>
{
};

/**
 * @note: 调用的是 _make_integer_sequence<T, size_t N, T...indxVals>
 * 这个类会递归生成一个新的类,直到N为0
 **/
template <typename... _Vals>
using index_sequence_for =
    _make_integer_sequence<std::size_t, sizeof...(_Vals)>;

template <std::size_t... IndxVals>
using index_sequence = integer_sequence<std::size_t, IndxVals...>;

template <typename Tuple, size_t... Indx>
constexpr void printImpl(const Tuple &t, index_sequence<Indx...>)
{
    (..., (std::cout << (Indx == 0 ? "" : ", ") << std::get<Indx>(t)));
}

template <typename... Args>
constexpr void print(const std::tuple<Args...> &t)
{
    printImpl(t, index_sequence_for<Args...>{});
}

} // namespace lap

void testForward()
{
    int &&Rightreferx = 1;
    int lx = 1;
    int &rlx = lx;
    const int cx = 1;

    lap::f(Rightreferx); // int
    lap::f(lx);          // int
    lap::f(rlx);         // int
    lap::f(cx);          // int

    lap::fforward(std::move(Rightreferx)); // int&&
    lap::fforward(lx);                     // int&
    lap::fforward(rlx);                    // int&
    lap::fforward(cx);                     // const int&
}

void testPrintTuple()
{
    auto t = std::make_tuple(1, 2, 3, "Hello!", 4.5);
    lap::print(t);
}

auto main(void) -> int
{
    testForward();
    testPrintTuple();
}