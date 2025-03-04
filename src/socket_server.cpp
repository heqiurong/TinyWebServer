/*
    写在前面的话：只是一个简单搭建的服务器，不支持并发处理多个客户端连接，不支持 HTTP 协议解析，只是简单的发送字符串消息。
    服务器使用 socket 编程，不依赖任何第三方库，使用阻塞 I/O 模型，每次只能处理一个客户端连接。
    * 一个简单的 C++ TCP 服务器示例
    * 该服务器监听 8080 端口，接受客户端连接后，向客户端发送 "Hello World!" 消息
*/
#include <sys/socket.h>         // 引入 socket 相关的库，提供创建套接字的功能
#include <netinet/in.h>          // 引入网络协议相关的库，提供 sockaddr_in 和地址转换等功能
#include <unistd.h>              // 引入 unix 标准库，提供 socket 操作函数如 close()
#include <iostream>              // 引入输入输出流库，用于打印日志
#include <cstring>               // 引入 C 字符串处理库，提供 strlen() 等函数

int main() {
    // 创建一个 TCP 套接字，返回的 server_fd 是服务器的套接字描述符
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    // sockaddr_in 是一个用于描述 IPv4 地址的结构体
    // 初始化一个空的 sockaddr_in 结构体，用于绑定服务器地址和端口
    sockaddr_in addr{};
    // 设置地址家族为 IPv4
    addr.sin_family = AF_INET; 
    // 绑定到所有可用的网络接口
    addr.sin_addr.s_addr = INADDR_ANY; 
    // 设置服务器监听端口为 8080，htons() 是为了保证端口字节序正确
    addr.sin_port = htons(8080);

    // 将套接字绑定到指定的地址和端口
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    
    // 设置监听队列长度为 5，表示最大同时等待连接的客户端数量
    listen(server_fd, 5);
    
    std::cout << "服务器监听端口8080...帅哥又来学习了。。。。这是帅哥的第一个服务\n";  // 打印服务器启动信息

    // 进入无限循环，等待客户端连接
    while (true) {
        // 接受客户端连接，client_fd 是与客户端通信的套接字
        int client_fd = accept(server_fd, nullptr, nullptr);
        
        // 定义 HTTP 响应消息
        const char* msg = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
        
        // 向客户端发送 HTTP 响应
        send(client_fd, msg, strlen(msg), 0);
        
        // 关闭与客户端的连接
        close(client_fd);
    }

    // 关闭服务器套接字
    close(server_fd); 

    return 0;  // 返回 0，表示程序正常结束
}
