/***
 * @author: "laplace"
 * @date: 2024-06-25T16:47:59
 * @lastmod: 2024-06-25T23:34:09
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/valElseRefer/main.cc
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 **/
#include <print>
#include <string_view>
#include <type_traits>
#include <utility>

namespace lap {

template < typename T >
void printV(T arg)
{
    std::println("{}", arg);
}

template < typename T >
void PrintVRefer(T &t)
{
    std::println("{}", t);
}

template < typename T1, typename T2 >
struct pair
{
    T1 first;
    T2 second;
};

template < typename T >
using Decay = std::decay_t< T >;

template < typename T1, typename T2 >
constexpr pair< Decay< T1 >, Decay< T2 > > make_pair(T1 &&t1, T2 &&t2)
{
    return pair< Decay< T1 >, Decay< T2 > >{std::forward< T1 >(t1),
                                            std::forward< T2 >(t2)};
}

} // namespace lap

void testPrintV()
{
    const int x = 27;
    // 按值传递发生类型decay

    lap::printV(x);
    lap::printV(27);
    lap::printV(27.01);
    lap::printV("27");
}

void testPrintVRefer()
{
    const int x           = 27;
    const double y        = 27.01;
    std::string_view z    = "27";
    const std::string str = "27";
    // 按引用传递不发生类型decay
    // 对于int、double这些类型,引用传递并不能提高性能,引用本身占用一个指针的大小
    // 对于string_view这种类型,引用传递可以提高性能
    int &&xr = sizeof(x);
    std::println("xr: {}", xr);
    lap::PrintVRefer(x);    // const int &
    lap::PrintVRefer(y);    // const double &
    lap::PrintVRefer("27"); // const char(&)[3]
    lap::PrintVRefer(z);    // std::basic_string_view &
    // lap::PrintVRefer(std::move(z)); // 不能接收xvalue,亡值
    lap::PrintVRefer(std::move(str)); // 被推导为左值引用,const导致了
}

template < typename T > concept isArray = std::is_array_v< T >;

template < isArray T >
void outR(T &arg)
{
    // 编译期断言,必须传入一个非const的引用
    static_assert(!std::is_const_v< T >, "out parameter of outR(T&) is const");
    std::println("{}", std::extent_v< T >); // 沿着某一个维度的元素个数,默认0维
}

template < typename T >
    requires(std::is_array_v< T > && !std::is_const_v< T >)
void outReq(T &arg)
{
    std::println("{}", std::extent_v< T >);
}

template < typename T >
void testRvalRefer(T &&)
{
    /***
     * @note: 需要注意引用折叠,当传入rvalue时,
     * 不发生折叠,则 T 被推导时不含有引用
     **/
    // 由于T &&是一个万能引用,所以可以接收左值和右值
    // 当传入左值时,T被推导为左值引用,所以val必须有初始化
    // T val;
}

template < typename T >
void testCref(T arg)
{
    /**
    @note: std::cref()返回一个std::reference_wrapper<T>对象
    该对象具有get方法,可以获取原始类型的引用
    **/
    std::println("{}", arg.get());
}

auto main() -> int
{
    int arr[]       = {1, 2, 3, 4, 5};
    const int x     = 1;
    std::string str = "123";
    testPrintV();
    testPrintVRefer();

    outR(arr);
    // outR("123");
    outReq(arr);
    // outReq("T &arg");

    testRvalRefer(arr);
    testRvalRefer(x);
    testRvalRefer(str);
    testRvalRefer(std::move(str));

    // 一个std::reference_wapper的对象,存在向原始类型的隐式转换
    // 必须让编译器知道需要将其转换为原始类型,才能正常使用
    testCref(std::cref(x));
    return 0;
}