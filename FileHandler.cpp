#include <iostream>
#include <memory>
#include <fstream>

class FileHandler {
public:
    // 构造函数获取文件句柄（关键点：RAII思想）
    explicit FileHandler(const std::string& filename) {
        file_.open(filename);  // 可能抛出异常
        if (!file_.is_open()) {
            throw std::runtime_error("文件打开失败");
        }
    }

    ~FileHandler() {
        if (file_.is_open()) {
            file_.close();  // 确保资源释放
        }
    }

    void write(const std::string& content) {
        file_ << content;
    }

private:
    std::ofstream file_;  // 文件流对象
};

int main() {
    try {
        // 使用unique_ptr管理FileHandler（关键点：异常安全）
        auto handler = std::make_unique<FileHandler>("test.txt");
        handler->write("Hello, C++11!");
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }
    return 0;
}
