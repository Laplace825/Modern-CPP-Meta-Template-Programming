/*
 * @Author: Laplace825
 * @Date: 2024-06-14 18:29:07
 * @LastEditTime: 2024-06-14 20:55:46
 * @LastEditors: Laplace825
 * @FilePath: \EffectiveModernCPP\GMP\variadic\main.cc
 * @Description:
 * Copyright (c) 2024 by Laplace825, All Rights Reserved.
 */

#include <print>
#include <tuple>
#include <vector>
#include <format>
#include <vector>
#include <unordered_set>

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
struct formatter<lap::Customer>
{
    // 解析格式规范（如果你需要的话）
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    // 格式化 Customer 对象
    template <typename FormatContext>
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

template <typename T>
void print(T arg)
{
    println("{}", arg);
}

template <typename T, typename... Args>
void print(T firstArg, Args... args)
{
    println("{}", firstArg);
    print(args...); // 每次把第二个参数及以后传入,直到最后一个参数
}

template <typename... Args>
void getArgsSize(Args... args)
{
    // sizeof...()返回参数包中的参数个数

    println("Args has {} elements", sizeof...(Args));
    println("args has {} elements", sizeof...(args));
}

template <typename T, typename... Args>
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
template <typename... Args>
void foldPrint(Args... args)
{
    println("[foldPrint]"); // 展开参数包
    (println("{}", args), ...);
}

template <typename... Args>
auto foldSum(Args... args)
{
    // 展开参数包,并求和,但是不能是空参数包
    if constexpr (sizeof...(args) == 0)
    {
        return 0;
    }
    println("[foldSum]:{}", (args + ...)); // 展开参数包
    return (args + ...);
}

struct Node
{
    int value;
    Node* left;
    Node* right;
    Node(int i = 0) : value(i), left(nullptr), right(nullptr) {}
};

auto left = &Node::left;
auto right = &Node::right;
// traverse tree, using fold expression:
template <typename T, typename... TP>
Node* traverse(T np, TP... paths)
{
    /**
    @brief: 按照paths 给定的路径进行遍历
     */
    return (np->*...->*paths); // np ->* paths1 ->* paths2 …
}

void createCustomers()
{
    std::vector<Customer> customers;
    customers.emplace_back("Laplace", 25, 1000.0);
    customers.push_back({"Laplace", 25, 1000.0});
    for (const auto& c : customers)
    {
        println("{}", c);
    }
}

template <typename... Args>
void doublePrint(const Args&... args)
{
    println("[doublePrint]");
    // 结果一致

    betterPrint((args + args)...);
    betterPrint(args + args...);
    (println("{}", args + args), ...);
}

template <typename T, typename... Args>
constexpr bool isSameType(T&&, Args&&...)
{
    return (std::is_same_v<Args, T> && ...);
}

template <typename Container, typename... Indexs>
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

template <std::size_t... Indexs, typename Container>
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

template <std::size_t... Idx, typename T>
void printByIdx(T t)
{
    println("[printByIdx]");
    (println("{}", std::get<Idx>(t)), ...);
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
        return std::hash<std::string>()(c.getName());
    }
};
// define class that combines operator() for variadic base classes:
template <typename... Bases>
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

    using CustomerOverloader = Overloader<CustomerEq, CustomerHash>;
    CustomerEq eq;
    CustomerHash hash;
    CustomerOverloader overloader;

    println("eq(c1,c2) = {}", eq(c1, c2));                 // true
    println("eq(c1,c3) = {}", eq(c1, c3));                 // false
    println("hash(c1) = {}", hash(c1));                    // hash value
    println("overloader(c1,c2) = {}", overloader(c1, c2)); // true
    println("overloader(c1) = {}", overloader(c1));        // hash value

    // 指定哈希函数,和比较函数
    std::unordered_set<Customer2, CustomerHash, CustomerEq> co1;

    std::unordered_set<Customer2, CustomerOverloader, CustomerOverloader> co2;
}

} // namespace lap

auto main() -> int
{
    lap::print(1, 2, 3, "hello", 3.14, 'c');

    lap::getArgsSize(1, 2, 3, "hello", 3.14, 'c');
    lap::betterPrint(1, 2, 3, "hello", 3.14, 'c');
    lap::foldPrint(1, 2, 3, "hello", 3.14, 'c');
    lap::foldSum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    lap::foldSum(1, 2.1, 3.1);
    lap::createCustomers();
    lap::doublePrint(1, 2, 3, 4, 5);

    // 展开为 std::is_smae_v<int 1,int 2> && std::is_same_v<int 1,int 3> && ...
    // 实际上没有数字在这里,只是为了方便理解
    println("is same ? {}", lap::isSameType(1, 2, 3, 4, 5));
    println("is same ? {}", lap::isSameType(1, 2, 3, 4, '1'));

    std::vector<int> vec = {1, 2, 3, 10, 1, 5};
    lap::printValuesTemplate<2, 0, 1>(vec); // 访问下标为0,2,1的元素

    std::vector<lap::Customer> cuss = {
        {"lap", 25, 1020.0}, {"jack", 22, 1100.0}, {"Alan", 20, 1500.0}};

    lap::printValues(cuss, 0, 2);

    auto tup = std::make_tuple(1, "hello", lap::Customer{"lap", 25, 1020.0});

    lap::printByIdx<1, 2>(tup);
    lap::testCustomer2();

    return 0;
}