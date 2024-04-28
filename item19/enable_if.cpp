#include <iostream>
#include <functional>
#include <type_traits>

template <typename T>
struct Func1
{
    using type = T;
};

template <size_t N>
struct Func2
{
    int data[N];
};

template <typename T>
typename std::enable_if<std::is_same_v<int, T>, T>::type
func(T x)
{
    std::cout << "func(T x)" << std::endl;
    return x;
}

int main()
{
    Func1<int>::type a = 10;
    Func2<10> b;
    func('1');
    return 0;
}