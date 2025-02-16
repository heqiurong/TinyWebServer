// LambdaDemo.cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};

    // 示例1：排序（降序）
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;  // 关键点：Lambda作为比较函数
    });

    // 示例2：过滤偶数
    nums.erase(std::remove_if(nums.begin(), nums.end(), 
        [](int x) { return x % 2 == 0; }), 
        nums.end()
    );

    // 输出结果：9 5 4 3 1 1 
    for (int num : nums) {
        std::cout << num << " ";
    }
    return 0;
}
