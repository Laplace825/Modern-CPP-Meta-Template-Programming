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
#include <concepts>
#include <iostream>
#include <print>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace lap {
template < typename X >
void g(X &)
{
    std::cout << "g() for variable\n";
}

template < typename X >
void g(X const &)
{
    std::cout << "g() for constant\n";
}

template < typename X >
void g(X &&)
{
    std::cout << "g() for movable object\n";
}

// let f() forward argument val to g():
// @note: 无法针对上面的f()进行重载,而且调用f()时,会根据传入的参数类型进行选择,
// 常发生值类型退化
template < typename X >
void f(X val)
{
    g(val);
};

template < typename T >
void fforward(T &&val)
{
    g(std::forward< T >(val));
}

template < typename T, size_t... _Vals >
struct integer_sequence
{
    using type                              = T;
    static constexpr std::size_t value_size = sizeof...(_Vals);
};

template < typename T, size_t N, T... _Vals >
struct _make_integer_sequence
    : _make_integer_sequence< T, N - 1, N - 1, _Vals... >
{
    /**
     * @brief: 递归生成序列
     * 每次递归都会生成一个新的继承类,直到N为0
     **/
};

template < typename T, T... _Vals >
struct _make_integer_sequence< T, 0, _Vals... >
    : integer_sequence< T, _Vals... >
{
};

/**
 * @note: 调用的是 _make_integer_sequence<T, size_t N, T...indxVals>
 * 这个类会递归生成一个新的类,直到N为0
 **/
template < typename... _Vals >
using index_sequence_for =
    _make_integer_sequence< std::size_t, sizeof...(_Vals) >;

template < std::size_t... IndxVals >
using index_sequence = integer_sequence< std::size_t, IndxVals... >;

template < typename Tuple, size_t... Indx >
constexpr void printImpl(const Tuple &t, index_sequence< Indx... >)
{
    (..., (std::cout << (Indx == 0 ? "" : ", ") << std::get< Indx >(t)));
    std::cout << '\n';
}

template < typename... Args >
constexpr void print(const std::tuple< Args... > &t)
{
    printImpl(t, index_sequence_for< Args... >{});
}

// enable_if
// bad practice
template < typename T >
typename std::enable_if< (sizeof(T) > 4), T >::type testEableIF(T &&t)
// since C++ 14
// std::enable_if_t<(sizeof(T)>4), T>
{
    std::println("enable_if :{}", sizeof(T));
    return std::forward< T >(t);
}

template < bool _Test, class _Tp = void >
using enable_if_t = typename std::enable_if< _Test, _Tp >::type;

template < typename T, typename T1 = std::enable_if_t< (sizeof(T) >= 4), T > >
T1 testEableIFIn(T &&t)
{
    std::println("enable_if in :{}", sizeof(T));
    return std::forward< T >(t);
}

class Person
{
  private:
    std::string m_name;

  public:
    operator std::string_view() { return m_name; }

    operator std::string() { return m_name; }

    // Person() = default;

    // since C++ 17
    template < typename SRC, typename = std::enable_if_t<
                                 std::is_convertible_v< SRC, std::string > > >
    explicit Person(SRC &&src) : m_name(std::forward< SRC >(src))
    {
        std::println("templ Person(SRC&&)");
    }

    // Person(const Person &other) : m_name(other.m_name)
    // {
    //     std::println("Person(const Person&)");
    // }

    Person(Person &&other) : m_name(std::move(other.m_name))
    {
        std::println("move Person(Person&&)");
    }

    // 禁用某些(特殊)成员函数
    // copy/move = 构造函数是不能被禁用的

    Person(const volatile Person &) =
        delete; // 这样会禁用拷贝构造函数,也不会隐式声明

    // 看另一个例子
    template < typename T,
               typename = std::enable_if_t< std::is_same_v< T, Person > > >
    Person(const T &t)
    {
        std::println("templ Person(const T&)");
    }
};

template < typename T >
class thisClass
{
  private:
    T m_t;

  public:
    thisClass(T t) : m_t(std::move(t)) {}

    thisClass(const volatile thisClass &) = delete;

    // 只有当T是整数类型时,才会生成这个构造函数
    template < typename U,
               typename = std::enable_if_t< std::is_integral_v< U > > >
    thisClass(const thisClass< U > &other) : m_t(other.m_t)
    {
        std::println("templ thisClass(const thisClass<U>&)");
    }
};

} // namespace lap

template < typename _FROM, typename... _TO >
concept ConvertiableTo = (std::is_convertible_v< _FROM, _TO > && ...);

template < typename _ValT, typename _FROM = _ValT, typename... _TO >
    requires ConvertiableTo< _FROM, _TO... >
void testConcept(const _ValT &t)
{
    std::println("concept test");
}

void testForward()
{
    int &&Rightreferx = 1;
    int lx            = 1;
    int &rlx          = lx;
    const int cx      = 1;

    lap::f(Rightreferx); // int
    lap::f(lx);          // int
    lap::f(rlx);         // int
    lap::f(cx);          // int

    lap::fforward(std::move(Rightreferx)); // int&&
    lap::fforward(lx);                     // int&
    lap::fforward(rlx);                    // int&
    lap::fforward(cx);                     // const int&
    lap::fforward(int{});
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
    auto some  = lap::testEableIF< double >(1.2);
    auto some1 = lap::testEableIFIn< int >(10);

    std::string str = "laplace";
    lap::Person p("laplace");
    lap::Person p1(str);
    lap::Person p2(std::move(p1));

    lap::thisClass tc("c");
    lap::thisClass< int > tint(1);

    // lap::thisClass t2(tc);

    lap::thisClass t3(tint);
    testConcept< lap::Person, std::string >(p1);
    return 0;
}