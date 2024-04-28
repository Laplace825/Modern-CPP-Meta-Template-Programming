#include <format>
#include <iostream>
#include <string>

using std::format;

template <typename Type>
Type add(Type a, Type b) {
    // NOTE: 一次定义多次调用
    std::cout << format("add({},{}) ", a, b);
    return a + b;
}

signed main() {
    std::cout << add(1, 2) << '\n';
    std::cout << add(1.1, 2.1) << '\n';
    std::cout << add<>(2, 5) << '\n';
    std::cout << add(std::string("Hello "), std::string("world!")) << '\n';

    int a = 1;
    double d = 2.2;
    // add(a, d);  // 报错 Type 无法推导

    add<double>(a, d);  // NOTE: 出现类型转换

    // add(); // 不能不实例化且不传入

    // NOTE: 没有参数无法推导
    // add<>();
    // add<int>();
}
