// TinyWebServer/main.cpp
#include <iostream>
#include <memory>  // 智能指针头文件

// 示例1：unique_ptr管理资源（关键点：自动释放内存）
void test_unique_ptr() {
    std::unique_ptr<int> ptr(new int(10));  // 独占所有权，禁止拷贝
    std::cout << *ptr << std::endl;         // 输出：10
    // ptr离开作用域时自动释放内存
}

// 示例2：lambda表达式（关键点：捕获列表与匿名函数）
void test_lambda() {
    int x = 5;
    auto lambda = [x](int y) -> int {      // 捕获外部变量x（值捕获）
        return x + y;
    };
    std::cout << lambda(3) << std::endl;   // 输出：8
}

// 示例3：基于范围的for循环（关键点：简化容器遍历）
void test_range_for() {
    int arr[] = {1, 2, 3};
    for (const auto& num : arr) {           // auto自动推导类型
        std::cout << num << " ";           // 输出：1 2 3
    }
}

int main() {
    test_unique_ptr();
    test_lambda();
    test_range_for();
    return 0;
}
