/*
    智能指针示例:unique_ptr和shared_ptr
    make_unique和make_shared是C++14引入的函数，用于创建智能指针对象。
    unique_ptr是独占所有权的智能指针(只能自己使用)，shared_ptr是共享所有权的智能指针(复制对象也可以使用)。
    在下面的示例中，我们创建了一个Database类，用于模拟数据库连接，然后使用unique_ptr和shared_ptr创建了两个智能指针对象。
    我们可以看到，当unique_ptr对象超出作用域时，数据库连接会被释放；而shared_ptr对象可以共享所有权，多个shared_ptr对象指向同一个数据库连接。
    创建时间:2025年3月
    创建人:何秋蓉
*/
#include <iostream>
#include <memory>

class Database {
public:
    Database() { std::cout << "数据库连接建立\n"; }
    ~Database() { std::cout << "数据库连接释放\n"; }
    void query(const std::string& sql) { 
        std::cout << "执行SQL: " << sql << "\n"; 
    }
    void this_is_a_test(int a) {
        std::cout << "this is :" << a << std::endl;
    }
};

int main() {
    // unique_ptr示例
    auto db1 = std::make_unique<Database>();
    db1->query("SELECT * FROM users");

    // shared_ptr示例
    auto db2 = std::make_shared<Database>();
    auto db3 = db2;  // 共享所有权
    db2->query("DELETE FROM logs");
    db3->query("INSERT INTO logs");
    db2->this_is_a_test(2);
    db3->this_is_a_test(3);
    return 0;
}
