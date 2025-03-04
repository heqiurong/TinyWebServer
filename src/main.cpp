#include "Socket.hpp"  // 自定义的Socket类头文件，需包含相关实现
#include <iostream>   // 标准输入输出流库
#include <memory>     // 提供智能指针支持

int main() {
    try {
        // 使用智能指针管理Socket对象（确保资源自动释放和异常安全）
        auto server = std::make_unique<Socket>();

        // 创建套接字
        server->create();
        // 绑定到8080端口
        server->bind(8080);
        // 开始监听连接请求
        server->listen();
        
        // 提示服务器启动信息
        std::cout << "服务器已启动，监听端口8080...。哥们也是开始学习了" << std::endl;
        
        //储存工作线程
        std::vector<std::thread> workers;
        // 进入主循环等待客户端连接
        while (true) {
            // TODO: 实现客户端连接的接收逻辑
            // 通常需要实现accept()方法和客户端处理逻辑
            // 注意：这部分内容将在下一周的课程中详细讲解
            int client_fd = server->accept();  
            // 等待并接受客户端连接请求，返回一个新的套接字client_fd，用于与客户端通信  

            workers.emplace_back(handle_client, client_fd);  
            // 创建一个新线程，处理当前客户端的请求，线程会调用handle_client函数，并传入client_fd  
        
            workers.back().detach(); // 分离线程  
            // 分离新创建的线程，使得线程能够在后台运行，当线程执行完毕后，自动释放资源  
        }
    } catch (const std::exception& e) {
        // 捕获并输出异常信息
        std::cerr << "致命错误: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
