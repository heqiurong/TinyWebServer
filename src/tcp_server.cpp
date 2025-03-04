/*
    * 一个简单的 TCP 服务器
    单线程，只能处理一个客户端连接
    创建时间：2025年3月3日
*/
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;              
    addr.sin_addr.s_addr = INADDR_ANY;      
    addr.sin_port = htons(8080);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    std::cout << "服务器监听端口8080...\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        const char* msg = "HTTP/1.1 200 OK\nContent-Length: 12\n\nHello World!";
        send(client_fd, msg, strlen(msg), 0);
        close(client_fd);
    }
    return 0;
}
