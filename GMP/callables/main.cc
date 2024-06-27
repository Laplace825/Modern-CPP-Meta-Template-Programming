/***
 * @author: "laplace"
 * @date: 2024-06-27T22:33:29
 * @lastmod: 2024-06-27T22:33:29
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/callables/main.c
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 ***/
#include <concepts>
#include <format>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <print>
#include <type_traits>
#include <utility>
#include <vector>

namespace lap {

// template < typename Inter, typename Callable >
// void for_each(Inter begin, Inter end, Callable &&op)
// {
//     while (begin != end)
//     {
//         std::forward< Callable >(op)(*begin);
//         ++begin;
//     }
// }

struct MyClass
{
    void memefunc(int i) { std::println("{}", i); }

    static void memefuncStatic(int i) { std::println("{}", i); }
};

// since C++17 invoke
template < typename Inter, typename Callable, typename... _Args >
void for_each(Inter begin, Inter end, Callable &&op, _Args &&...args)
{
    while (begin != end)
    {
        std::invoke(std::forward< Callable >(op), *begin,
                    std::forward< _Args >(args)...);
        ++begin;
    }
}

template < typename Callable, typename... _Args >
decltype(auto) Do(Callable &&op, _Args &&...args)
{
    return std::invoke(std::forward< Callable >(op),
                       std::forward< _Args >(args)...);
}

template < typename Callable, typename... _Args >
decltype(auto) Call(Callable &&op, _Args &&...args)
{
    if constexpr (std::is_void_v< std::invoke_result_t< Callable, _Args... > >)
        return std::invoke(std::forward< Callable >(op),
                           std::forward< _Args >(args)...);
    else
    {
        decltype(auto) res = std::invoke(std::forward< Callable >(op),
                                         std::forward< _Args >(args)...);
        return res;
    }
}

// 类型萃取
template < typename T >
struct Traits
{
    static_assert(!std::is_void_v< T >, "T can't be void");

  public:
    template < typename U >
    decltype(auto) foo(U &&u)
    {
        if constexpr (std::is_reference_v< U >)
        {
            std::println("is reference");
        }
        if constexpr (std::is_pointer_v< U >)
        {
            std::println("is pointer");
        }
        if constexpr (std::is_convertible_v< T, U > ||
                      std::is_convertible_v< U, T >)
        {
            std::println("is convertible");
        }
        return u;
    }
};

template < typename T, int &SZ > // 这里的SZ是一个引用,Bad
class Arr
{
  protected:
    std::vector< T > m_data;

  public:
    Arr() : m_data(SZ) {}

    void print() const
    {
        for (int i = 0; i < SZ; ++i)
        {
            std::print("{} ", m_data[i]);
        }
    }
};

template < bool _Condition, typename _Sucess, typename _Fail >
struct If
{
    using type = _Sucess;
};

template < typename _Sucess, typename _Fail >
struct If< false, _Sucess, _Fail >
{
    using type = _Fail;
};

template < bool _Condition, typename _Sucess, typename _Fail >
using If_t = typename If< _Condition, _Sucess, _Fail >::type;

} // namespace lap

void testFunObj()
{
    std::vector< int > vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    lap::for_each(vec.begin(), vec.end(),
                  [](const auto &elem) { std::print("{} ", elem); });

    lap::MyClass mc;
    // 调用成员函数
    lap::Do(&lap::MyClass::memefuncStatic, 1);
    lap::Do(&lap::MyClass::memefunc, mc, 1);
}

decltype(auto) testTraits()
{
    lap::Traits< int > t;
    lap::Traits< std::string > tstring;
    const char *str        = "Hello";
    const char *const cstr = "Hello";
    t.foo(1);
    t.foo("Hello");
    t.foo(str);  // 指针类型的引用
    t.foo(cstr); // 指针类型的引用
    t.foo(1.0);
    t.foo(tstring);
}

void testTypeCompare()
{
    std::decay_t< int const & > x1                = 1;
    std::add_rvalue_reference_t< int const > x2   = 1;
    std::add_lvalue_reference_t< int const & > x3 = x1; // 引用折叠
    if constexpr (std::is_copy_assignable_v< int >)
    {
        // 检查左值相关的, 一个int能否赋值给另一个
        std::println("int is copy assignable");
    }
    if constexpr (std::is_assignable_v< int, int >)
    {
        // false, 会把右值考虑来, 1=1 自然不行
        std::println("int is assignable");
    }
    if constexpr (std::is_assignable_v< int &, int & >)
    {
        // true, 引用之间当然可以
        std::println("int& is assignable");
    }
}

void testSomeUsefulTypeTraits()
{
    auto getAddress = [](auto &&any) { return std::addressof(any); };
    int x           = 1;
    std::cout << "Address of x: " << std::hex << getAddress(x) << '\n';
}

int size = 10;

void testTemplateRefer()
{
    lap::Arr< int, size > arr;
    arr.print();
    size += 10; // 导致了越界
    // arr.print(); // runtime error
}

template < typename Any >
lap::If_t< std::is_move_constructible_v< Any >, Any &&, Any & > testIF_t(
    Any &&arg)
{
    return std::forward< Any && >(arg);
}

auto main(void) -> int
{
    testFunObj();
    testTraits();
    testSomeUsefulTypeTraits();
    testTemplateRefer();
    int x       = 1;
    int &&prx   = 2;
    int &referx = x;
    testIF_t(1);

    testIF_t(x);
    testIF_t(referx);
    testIF_t(std::move(prx));

    const char *const str = "hello";
    std::string_view strv =  "hello";
    testIF_t("Hello");
    testIF_t(str);
    testIF_t(std::move(strv));
}