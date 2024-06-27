/***
 * @author: "laplace"
 * @date: 2024-06-27T18:27:49
 * @lastmod: 2024-06-27T18:40:09
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/compiler-time/main.cc
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 ***/

#include <array>
#include <functional>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace lap {

template < size_t _Num, size_t _Remain = 1, size_t _Residual = 1 >
    requires(_Num > _Residual)
struct IsPrime : public IsPrime< _Num, _Num % (_Residual + 1), _Residual + 1 >
{
};

template < size_t _Num, size_t _Remain >
    requires(_Remain > 0)
struct IsPrime< _Num, _Remain, _Num - 1 > : public std::true_type
{
};

template < size_t _Num, size_t _Residual >
    requires(_Num > _Residual)
struct IsPrime< _Num, 0, _Residual > : public std::false_type
{
};

template < size_t _Num >
constexpr auto is_prime_v = IsPrime< _Num >::value;

constexpr bool is_prime_func(size_t _Num, size_t _Residual = 1)
{
    return _Num / 2 > _Residual ? _Num % (_Residual + 1) == 0
                                      ? false
                                      : is_prime_func(_Num, _Residual + 1)
                                : true;
}

auto is_prime_func_bind = std::bind(is_prime_func, std::placeholders::_1, 1);

/***
 * @note: 类当中的偏特例化进行路径选择
 * 函数模版不支持部分特例化,最好使用enale_if或者concept
 ***/
template < size_t SZ, bool = is_prime_v< SZ > >
struct Helper;

template < size_t SZ >
struct Helper< SZ, true >
{ // SZ 为素数时的特例化
    static constexpr bool value = true;
};

template < size_t SZ >
struct Helper< SZ, false >
{ // SZ 为非素数时的特例化
    static constexpr bool value = false;
};

template < typename T, size_t SZ >
void testHelper(const std::array< T, SZ >&)
{
    std::println("is {} prime? {}", SZ, Helper< SZ >::value);
    if constexpr (!is_prime_v< SZ >)
    {
        std::println("is {} prime? ", SZ);
    }
}

/***
 * @note: SFINAE 替换失败不是错误
 *
 ***/
template < typename T, std::size_t N >
constexpr std::size_t len(T (&)[N])
{
    return N;
}

template < typename T >
constexpr typename T::size_type len(const T& t)
{
    return t.size();
}

constexpr std::size_t len(...) { return 0; }

struct Dump2FileClass
{
    void dump2File() const { std::println("class: dump2File"); }
};

template < typename T >
auto DumpObj(const T& t) -> decltype((std::declval< T >().dump2File(), void()))
{
    t.dump2File();
}

void DumpObj(...) { std::println("no dump2File function"); }

} // namespace lap

void testIsPrime()
{
    std::println("is 2 prime? {}", lap::is_prime_v< 2 >);

    std::println("is 3 prime? {}", lap::is_prime_v< 3 >);
    std::println("is 10 prime? {}", lap::is_prime_v< 10 >);
    std::println("is 10 prime? {}", lap::is_prime_func(10));
    std::println("is 2 prime? {}", lap::is_prime_func(2));
    std::println("is 10 prime? {}", lap::is_prime_func_bind(10));
}

void testHelper()
{
    std::array< int, 10 > arr;
    lap::testHelper(arr);
}

void testSFINAE()
{
    std::println("len \"123\":{}", lap::len("123"));
    std::println("len vector:{}", lap::len(std::vector< int >{1, 2, 4}));

    int* ptr = nullptr;
    lap::len(ptr);
    lap::DumpObj(lap::Dump2FileClass{});
    lap::DumpObj(2); // 不存在 dump2File 函数,将匹配到最差一个重载
}

auto main() -> int
{
    // testIsPrime();
    // testHelper();
    testSFINAE();
    return 0;
}