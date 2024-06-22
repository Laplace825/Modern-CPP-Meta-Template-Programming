/*
 * @Author: Laplace825
 * @Date: 2024-06-14 15:31:33
 * @LastEditTime: 2024-06-14 20:56:51
 * @LastEditors: Laplace825
 * @FilePath: \EffectiveModernCPP\GMP\no-type-param-template\main.cc
 * @Description:
 * Copyright (c) 2024 by Laplace825, All Rights Reserved.
 */
#include <array>
#include <initializer_list>
#include <print>
#include <string>
#include <type_traits>

using std::println;

namespace lap {

template <typename T, auto MaxSize>
class Array
{
public:
    using size_type = decltype(MaxSize);

private:
    std::array<T, MaxSize> m_data;
    size_type m_length;

public:
    constexpr Array() = default;
    constexpr Array(const std::initializer_list<T> &initList)
        : m_length(m_data.size())
    {
        std::copy(initList.begin(), initList.end(), m_data.begin());
    }

    constexpr size_type length() const { return m_length; }
};

// since C++ 17
template <typename T, auto Maxsize> // 使用auto作为非类型模版参数
class StackThisFile
{
public:
    using size_type = decltype(Maxsize);

private:
    std::array<T, Maxsize> elems; // elements
    size_type numElems;           // current number of elements
public:
    StackThisFile() = default; // constructor
    bool empty() const
    { // return whether the StackThisFile isempty
        return numElems == 0;
    }
    size_type size() const
    { // return current number of elements
        return numElems;
    }
};

// 函数模版非类型参数
template <int Val, typename T> // 不太好,因为T类型与Val不同,不一定能相加
T &addValue(T &x)
{
    x = x + Val;
    return x;
}

// since C++ 17
template <auto Val, typename T = decltype(Val)>
T &addValueAuto(T &x)
{
    // 保证两个类型一致
    x = x + Val;
    return x;
}

template <typename T, T Val = T()> // 不是很灵活
T &addValueDefault(T &x)
{
    x += Val;
    return x;
}

// template <double Val>
// double &addValueDouble(double &x)
// {
//     x += Val;
//     return x;
// } msvc clang
// 没有报错,但是调用时会报错,因为浮点数或类类型不能作为非类型模版参数

void testStackType()
{
    StackThisFile<int, 20u> intStack;
    StackThisFile<std::string, 40> stringStack;

    // 编译期检查
    if constexpr (!std::is_same_v<decltype(intStack)::size_type,
                                  decltype(stringStack)::size_type>)
    { // size type 一个是 usigned int, 一个是 int
        println("intStack size_type is unsigned int"
                "stringStack size_type is int");
    }
}

void testArrayType()
{
    Array<int, 20u> intArray;
    Array<std::string, 40> stringArray;
    // 编译期检查
    if constexpr (!std::is_same_v<decltype(intArray)::size_type,
                                  decltype(stringArray)::size_type>)
    { // size type 一个是 usigned int, 一个是 int
        println("intStack size_type is unsigned int"
                "stringStack size_type is int");
    }
}

template <auto T>
// parameter( since C++17)
class Message
{
public:
    static void print() { println("Message:{}", T); }
};

void testMessage()
{
    // 可以是静态的常量表达式 since C++17
    static const char theMsg[] = "hello";
    // 依然不能是浮点数或类类型

    Message<theMsg>::print();
    Message<42>::print();
}

template <decltype(auto) N>
class Nclass
{
public:
    using N_type = decltype(N);
};

} // namespace lap

template <typename T>
using reference_type = T *const;

int globalInt = 10;

void testRefer()
{
    static int constexprInt = 10;
    constexpr int const &i = constexprInt;
}

auto main() -> int
{
    constexpr lap::Array<int, 5> array = {1, 2, 3, 4, 5};
    constexpr auto arrsize = array.length();
    int integer = 10;
    char const *str = "hello";
    lap::addValue<10>(str); // bad idea
    println("add 10 integer :{:*^10}", integer);
    lap::addValue<10, int>(integer);
    println("add 10 integer :{:*^10}", integer);

    // lap::addValueAuto<1.10>(integer);// 类型不匹配
    lap::addValueAuto<1>(integer);
    println("add 1 integer :{:*^10}", integer);

    double floatNumber = 1.02;
    // lap::addValueDefault<int, 10>(integer);
    lap::addValueDefault<int>(integer);
    println("add integer :{:*^10}", integer);
    // lap::addValueAuto<10.2>(floatNumber); //
    // 浮点数和类类型都不能作为非类型模版参数限制

    // lap::addValueDouble<10.2>(floatNumber);
    lap::testStackType();
    lap::testMessage();
    lap::testArrayType();

    static int constexprInt = 10;
    constexpr int &i = globalInt;

    lap::Nclass<i> n;

    // testRefer(integer);
    return 0;
}