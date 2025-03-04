/*
    Lambda表达式
    Lambda表达式是一种匿名函数，可以用来创建函数对象。
    Lambda表达式的基本形式如下：
    [捕获列表](参数列表) -> 返回类型 { 函数体 }
    创建时间：2025年3月
*/
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<std::string> words = {"apple", "banana", "cherry"};
    
    // 按长度排序（Lambda表达式）
    std::sort(words.begin(), words.end(), 
        [](const auto& a, const auto& b) { return a.size() < b.size(); });
    
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    return 0;
}