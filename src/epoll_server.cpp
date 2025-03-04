/*
    epoll: 高性能 I/O 多路复用模型
    1. 创建 Socket 并绑定
    2. 创建 Epoll 实例
    3. 将服务器 Socket 加入 Epoll 监听
    4. 事件循环
    创建时间: 2025年3月3日
*/
#include <sys/epoll.h>  // 引入 epoll 相关的头文件
#include <sys/socket.h>  // 引入 socket 相关的头文件
#include <netinet/in.h>  // 引入 IP 地址和端口等定义
#include <unistd.h>  // 引入 UNIX 标准函数头文件
#include <iostream>  // 引入标准输入输出流头文件
#include <vector>  // 引入 STL vector 容器类

#define MAX_EVENTS 1024  // 定义最大事件数，表示最多监听 1024 个事件

int main() {
    // 1. 创建 Socket 并绑定
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket创建失败\n";
        return -1;
    }
    
    sockaddr_in addr{};  
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "绑定失败\n";
        return -1;
    }
    
    if (listen(server_fd, 5) == -1) {
        std::cerr << "监听失败\n";
        return -1;
    }

    // 2. 创建 Epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll创建失败\n";
        return -1;
    }

    // 3. 将服务器 Socket 加入 Epoll 监听
    struct epoll_event ev;  // 定义 epoll_event 结构体，用来描述事件
    ev.events = EPOLLIN;  // 设置监听可读事件（客户端发送数据）
    ev.data.fd = server_fd;  // 设置监听的文件描述符为服务器 Socket

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {  // 注册事件到 epoll 实例
        std::cerr << "Epoll注册失败\n";
        return -1;
    }

    // 4. 事件循环
    std::vector<epoll_event> events(MAX_EVENTS);  // 定义一个 vector，存储监听到的事件

    while (true) {
        int nfds = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);  // 等待事件发生
        if (nfds == -1) {  // 如果发生错误，输出错误信息并退出
            std::cerr << "Epoll等待失败\n";
            return -1;
        }

        // 遍历所有发生的事件
        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;  // 获取文件描述符

            if (fd == server_fd) {  // 如果事件是服务器 Socket（表示有新连接）
                // 处理新连接
                int client_fd = accept(server_fd, nullptr, nullptr);
                if (client_fd == -1) {  // 如果接收连接失败，输出错误并跳过
                    std::cerr << "接受连接失败\n";
                    continue;
                }
                
                ev.events = EPOLLIN | EPOLLET;  // 设置为可读事件，并启用边缘触发（EPOLLET）
                ev.data.fd = client_fd;  // 设置客户端文件描述符
                
                // 将客户端 Socket 加入 epoll 监听
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    std::cerr << "客户端连接注册失败\n";
                    continue;
                }
            } else {  // 如果事件是客户端 Socket（表示客户端发送数据）
                // 处理客户端请求
                char buffer[1024];  // 定义一个缓冲区，用来接收客户端数据
                ssize_t len = recv(fd, buffer, sizeof(buffer), 0);  // 接收客户端数据
                if (len > 0) {
                    send(fd, "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello Epoll!", 46, 0);
                }
                close(fd);
            }
        }
    }

    close(server_fd);  // 关闭服务器 Socket
    return 0;  // 程序结束
}
