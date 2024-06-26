/***
 * @author: "laplace"
 * @date: 2024-06-20T14:09:35
 * @lastmod: 2024-06-20T14:30:47
 * @description:
 * @filePath: /EffectiveModernCPP/GMP/typename/main.cc
 * @lastEditor: Laplace825
 * @Copyright: Copyright (c) 2024 by Laplace825, All Rights Reserved.
 **/
#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace lap {

using std::begin;
using std::end;

template < typename T >
struct is_iterable
{
    using type =
        decltype((begin(std::declval< T& >()) !=
                      end(std::declval< T& >()), // begin/end and operator !=
                  void(),                        // Handle evil operator ,
                  ++std::declval< decltype(begin(
                      std::declval< T& >()))& >(),    // operator++
                  void(*begin(std::declval< T& >())), // operator*
                  std::true_type{}));
};

template < typename _Container >
using is_iterable_t = typename is_iterable< _Container >::type;

template < typename Container >
    requires(std::is_same_v< is_iterable_t< Container >, std::true_type >)
void printcoll(const Container& coll)
{
    typename Container::const_iterator pos;
    typename Container::const_iterator end(coll.end());
    for (pos = coll.begin(); pos != end; ++pos)
    {
        std::print("{} ", *pos);
    }
}

template < typename T >
void valInit()
{
    /**
    @brief: 用于初始化T类型的变量,直接调用即可
     */
    T val{};
}

template < typename T >
class Base
{
  public:
    void hello() { std::println("hello"); }
};

template < typename T >
class Derived : public Base< T >
{
  public:
    void say()
    {
        Base< T >::hello();
        this->hello();
        // hello(); // this is a bad practice
        // msvc 可以通过,但是gcc不行(msvc 会往基类查找)
    }
};

template < typename T, size_t M, size_t N >
bool less_than(T (&arr_1)[M], T (&arr_2)[N])
{
    for (size_t i = 0; i < M && i < N; ++i)
    {
        if (arr_1[i] < arr_2[i])
        {
            return true;
        }
        if (arr_2[i] < arr_1[i])
        {
            return false;
        }
    }
    return M < N;
}

template < typename T >
struct MyClass; // 主模板
                //

template < typename T, std::size_t SZ >
struct MyClass< T[SZ] > // partial specialization for arrays of known
{
    static void print() { std::cout << "print() for T[" << SZ << "]\n"; }
};

template < typename T, std::size_t SZ >
struct MyClass< T (&)[SZ] > // partial spec. for references to arrays of
{
    static void print() { std::cout << "print() for T(&)[" << SZ << "]\n"; }
};

template < typename T >
struct MyClass< T[] > // partial specialization for arrays of unknown
{
    static void print() { std::cout << "print() for T[]\n"; }
};

template < typename T >
struct MyClass< T (&)[] > // partial spec. for references to arrays of
{
    static void print() { std::cout << "print() for T(&)[]\n"; }
};

template < typename T >
struct MyClass< T* > // partial specialization for pointers
{
    static void print() { std::cout << "print() for T*\n"; }
};

template < typename T1, typename T2, typename T3 >
void foo(int a1[7], int a2[], // pointers by language rules
         int (&a3)[42],       // reference to array of known bound
         int (&x0)[],         // reference to array of unknown bound
         T1 x1,               // passing by value decays
         T2& x2, T3&& x3)     // passing by reference
{
    MyClass< decltype(a1) >::print();
    MyClass< decltype(a2) >::print();
    MyClass< decltype(a3) >::print();
    MyClass< decltype(x0) >::print();
    MyClass< decltype(x1) >::print();
    MyClass< decltype(x2) >::print();
    MyClass< decltype(x3) >::print();
}

template < typename T >
class Stack
{
    // 将所有类型的Stack类声明为友元类
    template < typename >
    friend class Stack;

  protected:
    std::deque< T > m_elems;

  public:
    void push(const T& elem);
    T pop();
    const T& top() const;
    bool empty() const;

    Stack(std::initializer_list< T > list) : m_elems(list) {}

    template < typename T2 >
    Stack& operator=(Stack< T2 > other)
    {
        m_elems.clear();
        std::move(other.m_elems.begin(), other.m_elems.end(),
                  std::back_inserter(m_elems));
        return *this;
    }

    void show() const
    {
        for (auto& elem : m_elems)
        {
            std::print("{} ", elem);
        }
        std::println("");
    }
}; // class Stack

class BoolString
{
  private:
    std::string m_str;

  public:
    BoolString(std::string str) : m_str(std::move(str))
    {
        std::println("BoolString constructor");
    }

    template < typename T >
    T get() const
    {
        return m_str;
    }
}; // class BoolString

/**
@note: 成员模版的特例化
必须被声明inline, 因为只能定义他们,而不能声明
 */
template <>
inline bool BoolString::get< bool >() const
{
    return m_str == "true" || m_str == "false";
}

template < unsigned long N >
void printBitset(const std::bitset< N >& bs)
{
    /**
    @note: 显式地调用成员模版,避免解析为比较运算符
     */
    std::cout << bs.template to_string< char, std::char_traits< char >,
                                        std::allocator< char > >();
}

// 被编译成了一个仿函数
constexpr auto lambdaTemplatePrint = [](auto... args) -> void {
    std::cout << "lambdaTemplatePrint: ";
    ((std::cout << args << " "), ...);
    std::cout << '\n';
};

template < typename... T >
auto som = [](T... args) {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
};

struct SimulateFunction
{
    template < typename... Args >
    static auto operator()(Args... args)
    {
        std::cout << "SimulateFunction: ";
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
    }
};

// Since  C++ 14
template < typename T >
constexpr T pi{3.1415926535897932385L};

template < typename T, auto N >
struct testTemplaVal
{
    // template<auto N = size_t{}>
    // std::array<int, N> arr{}; // 不允许类成员变量是模板

    // good practice
    static std::array< int, N > arr{};

    static decltype(N) dval = N;
};

// since C++ 14
template < auto N = int{} >
constexpr decltype(N) dval = N;

template < auto N = size_t{} >
std::array< int, N > arr{};

// 利用变量模版,将一些复杂的类中常量提取出来
template < typename T = size_t >
struct MyContainer
{
    static size_t value;
    using value_type = T;

    MyContainer() { value++; }
};

template < typename T >
size_t MyContainer< T >::value = 0;

// 这就是标准库经常使用的技巧
template < typename T >
auto myContainer_v = MyContainer< T >::value;

// 注意,上面是对一个值的提取,下面是对一个类型的提取
template < typename T >
using myContainer_vt = MyContainer< T >::value_type;

/**
@note: 模版参数模版
 */
template < typename T,
           template < typename Elem, typename... > typename Cont = std::vector >
// 从 C++ 17 开始,可以使用 typename Cont = std::vector
// 之前则必须 class Cont = std::vector
// template <typename T,
//           template <typename Elem, typename Allocator = std::allocator<Elem>>
//           class Cont = std::vector>
class AStack
{
    // 将所有实例类声明为友元类
    template < typename, template < typename, typename... > typename >
    friend class AStack;

  private:
    Cont< T > elems; // elements
  public:
    using iterator_type        = typename Cont< T >::iterator;
    iterator_type iterator     = elems.begin();
    iterator_type end_iterator = elems.end();

    using const_iterator_type         = typename Cont< T >::const_iterator;
    const_iterator_type citerator     = elems.cbegin();
    const_iterator_type cend_iterator = elems.cend();

    AStack(std::initializer_list< T > list) : elems(std::move(list)) {}

    void show()
    {
        for (iterator_type it = iterator; it != end_iterator; ++it)
        {
            std::cout << *it << ' ';
        }
    }
};

template < typename T >
using astack_const_iterator_v = typename AStack< T >::const_iterator_type;

template < typename T >
using astack_iterator_v = typename AStack< T >::iterator_type;

} // namespace lap

void testMyContainer()
{
    lap::MyContainer< int > c1;
    lap::MyContainer< char > cc2;
    lap::MyContainer< char > cc3;
    std::cout << lap::myContainer_v< int > << '\n';
    std::cout << lap::myContainer_v< char > << '\n';
    std::cout << lap::MyContainer< char >::value << '\n';
}

auto main(void) -> int
{
    std::vector< std::string_view > strs{"hello", "world", "laplace"};

    lap::printcoll(strs);

    lap::Derived< int > s;
    s.say();

    lap::less_than("Hello", "word");

    int a[42];
    lap::MyClass< decltype(a) >::print(); // uses MyClass<T[SZ]>
    int x[] = {1, 2, 3};                  // forward declare array
    lap::MyClass< decltype(x) >::print(); // uses MyClass<T[]>
    lap::foo(a, a, a, x, x, x, x);

    lap::Stack< int > stk_1{1, 2, 3, 4, 5};
    lap::Stack< double > stk_2{6.2, 7.9, 8.0, 9.0, 10.0};
    stk_1 = stk_2;
    stk_1.show();
    stk_2.show();
    lap::lambdaTemplatePrint("hello", "world", "laplace", 825);

    lap::SimulateFunction::operator()("hello", "world", "laplace");

    auto cAscii = lap::dval< 'c' >;
    std::cout << cAscii << std::endl;

    // since C++ 17
    static constexpr char str[] = "hello\n";
    std::cout << lap::dval< str >;

    // 实例化了一个长度为10的数组
    lap::arr< 10 >.at(0) = 10;

    // std::cout << lap::template myContainer_v<int> << '\n';

    lap::AStack< int > vStack{1, 2, 3, 4, 5};
    vStack.show();
    testMyContainer();
    std::map< int, std::string > p{
        {1, "hello"  },
        {2, "world"  },
        {3, "laplace"}
    };
}