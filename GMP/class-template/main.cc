#include <deque>
#include <initializer_list>
#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace lap {

template <typename T>
class Stack;

template <typename T>
std::ostream &operator<<(std::ostream &, Stack<T> const &);

template <typename T>
class Stack
{
    // @note: 重载时,这里必须声明为一个特化的函数模板
    // 因为在重载的定义当中，我们必须使用template进行定义
    friend std::ostream &operator<< <>(std::ostream &, Stack<T> const &);

private:
    std::vector<T> m_data;

public:
    Stack() = default;
    Stack(Stack const
              &); // 参数不指定模板特化类型,默认与当前被构造的Stack类型一致
    Stack(Stack<T> &, size_t size);
    Stack(T elem) : m_data({std::move(elem)}) {}
    Stack(const std::initializer_list<T> &initList) : m_data(initList) {}

    void pop()
    {
        m_data.pop_back();
        /**
        @note: 在C++中，实现一个既异常安全又能返回被删除元素的 `pop()`
方法是具有挑战性的，原因在于以下几点：
1.
**异常安全保证**：在C++中，异常安全通常意味着即使在异常发生时，
程序的状态也保持一致，不会泄露资源或导致数据损坏。
异常安全可以分为三个级别：基本保证、强保证和不抛异常保证。
2.
**返回被删除元素**：如果`pop()`方法设计为返回被删除的元素（而不是通过引用或指针传递），
那么在返回过程中，对象需要被复制或移动。这个复制或移动操作可能会抛出异常。
3.
**异常发生时的状态一致性**：如果在`pop()`方法中，元素被从容器中删除（例如，从栈顶移除），
然后在返回该元素的过程中发生异常（例如，复制构造函数抛出异常），那么容器已经改变了状态（元素已
被删除），但是调用者无法接收到被删除的元素。这就破坏了异常安全的基本保证，
因为操作没有完成，但对象的状态已经改变。为了解决这个问题，通常的做法是将`pop()`方法分解为两个操作：
@note:
也就是说比如调用pop,然后栈顶指针肯定在返回元素之前后退,然后再返回元素,但是一旦返回元素时发生异常,
则无法恢复栈顶位置，也可能无法把原来的数据进行恢复。
-
一个是`top()`方法，返回容器顶部元素的引用，但不删除它。这个操作可以是异常安全的，
因为它不涉及元素的复制或移动。
-
另一个是`pop()`方法，仅负责删除顶部元素，不返回它。由于不需要返回元素，
就不存在复制或移动可能引发异常的问题。
这种设计方式可以确保即使在异常发生时，容器的状态也保持一致，从而满足异常安全的要求。
         */
    }

    void printOn(std::ostream &strm) const
    {
        for (T const &elem : m_data)
        {
            strm << elem << ' '; // call << for each element
        }
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, Stack<T> const &stk)
{
    for (auto &elem : stk.m_data)
    {
        out << elem << ' ';
    }
    out << std::endl;
    return out;
}

// 模板特例化
template <typename T>
class Stack<T *>
{
private:
    std::deque<T *> m_data;

public:
    Stack(T *elem) { m_data.push_back(elem); }
    void print() { std::println("Stack<T*>"); }
}; // 一个专门处理指针的特例,可以连底层实现都完全不同

// 要求将char const * 推导为std::string
/**
    @brief: 其实也就是显式要求针对某种构造函数，需要构造哪种类型的模版实例
    例如针对使用 char const * 的构造，需要构造一个std::string的模版实例
**/
Stack(char const *) -> Stack<std::string>;
Stack(char *const) -> Stack<std::string>;
Stack(std::initializer_list<const char *>) -> Stack<std::string>;

void DedutionGuides()
{
    // 如果没有上面的推断指引,那么会推导为char const * (按值传递)
    // char const * 和 const char * 都是顶层const
    Stack stk("Hello"); // Stack<std::string>
    /**
    @note: A PROBLEM HAPPEN!
    这里的stk在初始化时，如果使用小括号()，那么可以执行推断指引
    而使用{}，则没有发生推断指引
    发现是由于定义了初始化列表导致的构造问题。
    **解释**：
    1. 构造函数需要优先匹配最优的那个，这里即匹配到了初始化列表
    2. 如果特例化了一个对指针做处理的类，例如Stack<T*>,
    那么这里stk_other匹配了这个类的构造函数
    **/
    Stack stk_other{"Hello"};
    int *p = new int[7];
    Stack stk_ptr(p);
    stk_ptr.print();
    delete[] p;
}

// 聚合类
template <typename T>
struct ValueWithComment
{
    T value;
    std::string comment;
};

template <typename T>
ValueWithComment(T, const char *) -> ValueWithComment<T>;

void testValueWithComment()
{
    ValueWithComment valWizComet{.value = 10, .comment = "hlo"};

    // 当没有显示指明类型推断指引时，无法使用=的初始化构造
    ValueWithComment valWizComet1 = {10, "hlo"};
}

// 部分模板特例化
template <typename T1, typename T2>
class TwoTemplateClass
{
};

template <typename T>
class TwoTemplateClass<T, T>
{
};

template <typename T>
class TwoTemplateClass<T, int>
{
};

} // namespace lap

auto main() -> int
{
    {
        lap::Stack<int> stk;

        class ILOVEU; // 非模板类可以在块作用域内直接声明或定义

        // template <typename T> // 模板类不能在块作用域内声明或定义
        // class lap::Stack;
    }

    lap::Stack<int> stk = {1, 2, 10};
    std::cout << stk;
    lap::DedutionGuides();
}