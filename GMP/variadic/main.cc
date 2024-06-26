/*
 * @Author: Laplace825
 * @Date: 2024-06-14 18:29:07
 * @LastEditTime: 2024-06-14 20:55:46
 * @LastEditors: Laplace825
 * @FilePath: \EffectiveModernCPP\GMP\variadic\main.cc
 * @Description:
 * Copyright (c) 2024 by Laplace825, All Rights Reserved.
 */

#include <format>
#include <print>
#include <stdexcept>
#include <tuple>
#include <unordered_set>
#include <vector>


using std::println;

namespace lap {
struct Customer
{
    std::string name;
    int age;
    double money;
};
} // namespace lap

namespace std {
template <>
struct formatter< lap::Customer >
{
    // 解析格式规范（如果你需要的话）
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    // 格式化 Customer 对象
    template < typename FormatContext >
    auto format(const lap::Customer& c,
                FormatContext& ctx) const -> decltype(ctx.out())
    {
        // 假设 Customer 类有 name, age, balance 属性
        return format_to(ctx.out(), "Name: {}, Age: {}, Balance: {}", c.name,
                         c.age, c.money);
    }
};
} // namespace std

namespace lap {

template < typename T >
void print(T arg)
{
    println("{}", arg);
}

template < typename T, typename... Args >
void print(T firstArg, Args... args)
{
    println("{}", firstArg);
    print(args...); // 每次把第二个参数及以后传入,直到最后一个参数
}

template < typename... Args >
void getArgsSize(Args... args)
{
    // sizeof...()返回参数包中的参数个数

    println("Args has {} elements", sizeof...(Args));
    println("args has {} elements", sizeof...(args));
}

template < typename T, typename... Args >
void betterPrint(T firstArg, Args... args)
{
    println("{}", firstArg);
    if constexpr (sizeof...(Args) > 0) // since C++ 17
    {
        // 必须在编译时确定,因为如果是运行时确定,那么编译器就无法确定是否会执行这个分支
        // 例如只传一个参数,此时缺少无参数的betterPrint定义
        betterPrint(args...);
    }
}

// since C++ 17 折叠表达式
template < typename... Args >
void foldPrintLeft(Args... args)
{
    println("[foldPrintLeft]"); // 展开参数包
    (println("{}", args), ...);
}

template < typename... Args >
void foldPrintRight(Args... args)
{
    println("[foldPrintRight]"); // 展开参数包
    (..., println("{}", args));
}

void testKuohao()
{
    // 测试括号表达式
    int a{0}, b{0}, c{0};
    println("{}", (a = 1, b = 2, a + b + c));
}

template < typename T, typename... Args >
auto foldSum(T firstArg, Args... args)
{
    if constexpr ((std::is_same_v< T, Args > && ...))
    {
        auto res = (args + ... + firstArg);
        println("[foldSum]:{}", res); // 展开参数包
        return res;
    }
    else
    {
        throw std::runtime_error(
            "please input the same type of arguments to sum");
    }
}

struct Node
{
    int value;
    Node* left;
    Node* right;

    Node(int i = 0) : value(i), left(nullptr), right(nullptr) {}
};

auto left  = &Node::left;
auto right = &Node::right;

// traverse tree, using fold expression:
template < typename T, typename... TP >
Node* traverse(T np, TP... paths)
{
    /**
    @brief: 按照paths 给定的路径进行遍历
     */
    return (np->*...->*paths); // np ->* paths1 ->* paths2 …
}

void createCustomers()
{
    std::vector< Customer > customers;
    customers.emplace_back("Laplace", 25, 1000.0);
    customers.push_back({"Laplace", 25, 1000.0});
    for (const auto& c : customers)
    {
        println("{}", c);
    }
}

template < typename... Args >
void doublePrint(const Args&... args)
{
    println("[doublePrint]");
    // 结果一致

    betterPrint((args + args)...);
    betterPrint(args + args...);
    (println("{}", args + args), ...);
}

template < typename T, typename... Args >
constexpr bool isSameType(T&&, Args&&...)
{
    return (std::is_same_v< Args, T > && ...);
}

template < typename Container, typename... Indexs >
void printValues(const Container& container, Indexs... indexs)
{
    // lambda表达式,检查下标是否合法
    // 相当于 python all()
    auto isIndexValid = [](auto size, auto... index) {
        return ((index < size) && ...);
    };

    if (!isIndexValid(container.size(), indexs...))
    {
        throw std::out_of_range("index out of range");
    }
    // 展开参数包,并打印对应的值
    println("[printValues]");

    (println("{}", container[indexs]), ...);
}

template < std::size_t... Indexs, typename Container >
void printValuesTemplate(const Container& container)
{
    // lambda表达式,检查下标是否合法
    // 相当于 python all()
    auto isIndexValid = [](auto size, auto... index) {
        return ((index < size) && ...);
    };

    if (!isIndexValid(container.size(), Indexs...))
    {
        throw std::out_of_range("index out of range");
    }
    // 展开参数包,并打印对应的值
    println("[printValuesTemplate]");

    (println("{}", container[Indexs]), ...);
}

template < std::size_t... Idx, typename T >
void printByIdx(T t)
{
    println("[printByIdx]");
    (println("{}", std::get< Idx >(t)), ...);
}

template < std::size_t... >
struct IndexSequence
{
};

template < typename T, std::size_t... Idx >
void printValueWithIndices(T t, IndexSequence< Idx... >)
{
    println("[printValueWithIdx]");
    (println("{}", std::get< Idx >(t)), ...);
}

class Customer2
{
  private:
    std::string name;

  public:
    Customer2(std::string const& n) : name(n) {}

    std::string getName() const { return name; }
};

struct CustomerEq
{
    bool operator()(Customer2 const& c1, Customer2 const& c2) const
    {
        return c1.getName() == c2.getName();
    }
};

struct CustomerHash
{
    std::size_t operator()(Customer2 const& c) const
    {
        return std::hash< std::string >()(c.getName());
    }
};

// define class that combines operator() for variadic base classes:
template < typename... Bases >
struct Overloader : Bases...
{
    /**
    @note: 相当于对所有的基类的operator()进行重载
    在这个customer2的例子中，重载了两个参数和一个参数的operator()
    两个参数刚好对应于相等
    一个参数对应于哈希
     */
    using Bases::operator()...; // OK since C++17
};

void testCustomer2()
{
    Customer2 c1("Jack");
    Customer2 c2("Jack");
    Customer2 c3("Laplace");

    using CustomerOverloader = Overloader< CustomerEq, CustomerHash >;
    CustomerEq eq;
    CustomerHash hash;
    CustomerOverloader overloader;

    println("eq(c1,c2) = {}", eq(c1, c2));                 // true
    println("eq(c1,c3) = {}", eq(c1, c3));                 // false
    println("hash(c1) = {}", hash(c1));                    // hash value
    println("overloader(c1,c2) = {}", overloader(c1, c2)); // true
    println("overloader(c1) = {}", overloader(c1));        // hash value

    // 指定哈希函数,和比较函数
    std::unordered_set< Customer2, CustomerHash, CustomerEq > co1;

    std::unordered_set< Customer2, CustomerOverloader, CustomerOverloader > co2;
}

template < typename... ElemTypes >
class Tuple
{
};

template < typename T >
class Tuple< T >
{
  public:
    using base_type = Tuple<>;
    using this_type = Tuple< T >;
    using data_type = T;

    constexpr Tuple(data_type&& value)
        : m_value(std::forward< data_type >(value))
    {
    }

    data_type& get() { return m_value; }

  protected:
    data_type m_value;
};

template < typename T, typename... ElemTypes >
class Tuple< T, ElemTypes... > : private Tuple< ElemTypes... >
{
  public:
    using base_type = Tuple< ElemTypes... >;
    using this_type = Tuple< T, ElemTypes... >;
    using data_type = T;

    constexpr Tuple(data_type&& value, ElemTypes&&... values)
        : base_type(std::forward< ElemTypes >(values)...),
          m_value(std::forward< data_type >(value))
    {
    }

    using Tuple< ElemTypes... >::get;
    // data_type& get() { return m_value; }

  protected:
    data_type m_value;
};

template < typename... Args >
auto makeTuple(Args&&... args)
{
    return Tuple< Args... >(std::forward< Args >(args)...);
}

} // namespace lap

auto main() -> int
{
    std::string str  = "hello";
    std::string str2 = "world";
    std::string str3 = "CPP";
    println("{}", lap::foldSum(str, str2, str3));
    // lap::print(1, 2, 3, "hello", 3.14, 'c');

    // lap::getArgsSize(1, 2, 3, "hello", 3.14, 'c');
    // lap::betterPrint(1, 2, 3, "hello", 3.14, 'c');
    // lap::foldPrintLeft(1, 2, 3, "hello", 3.14, 'c');
    // lap::foldPrintRight(1, 2, 3, "hello", 3.14, 'c');
    // lap::foldSum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    // lap::foldSum(1, 2.1, 3.1);
    // lap::createCustomers();
    // lap::doublePrint(1, 2, 3, 4, 5);

    // // 展开为 std::is_smae_v<int 1,int 2> && std::is_same_v<int 1,int 3> &&
    // ...
    // // 实际上没有数字在这里,只是为了方便理解
    // println("is same ? {}", lap::isSameType(1, 2, 3, 4, 5));
    // println("is same ? {}", lap::isSameType(1, 2, 3, 4, '1'));

    std::vector< int > vec = {1, 2, 3, 10, 1, 5};
    lap::printValuesTemplate< 2, 0, 1 >(vec); // 访问下标为0,2,1的元素

    // std::vector<lap::Customer> cuss = {
    //     {"lap", 25, 1020.0}, {"jack", 22, 1100.0}, {"Alan", 20, 1500.0}};

    // lap::printValues(cuss, 0, 2);

    auto tup = std::make_tuple(1, "hello", lap::Customer{"lap", 25, 1020.0});
    lap::printByIdx< 1, 2, 0 >(tup);
    lap::printValueWithIndices(tup, lap::IndexSequence< 0, 1, 2 >());
    // lap::testCustomer2();

    // lap::testKuohao();
    return 0;
}