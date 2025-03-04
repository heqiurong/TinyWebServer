/*
    * multi_thread_server.cpp
    *TinyWebServer/src/multi_thread_server.cpp  
    *创建时间: 2025年3月3日
    这是一个多线程的Web服务器，使用C++11标准库中的线程支持，实现了一个简单的HTTP服务器，可以处理多个客户端的请求。
    服务器监听在8080端口上，接收客户端的连接请求，每当有新的客户端连接时，就创建一个新的线程来处理客户端的请求。
    服务器接收到客户端的请求后，会构造一个简单的HTTP响应，包括状态码200和内容“Hello World!”，然后将响应数据发送回客户端。
*/
#include <sys/socket.h>  
#include <netinet/in.h>  
// 引入inet协议族相关头文件，定义网络字节序等常量  
#include <unistd.h>  
// 引入unistd.h头文件，提供close()等系统调用函数  
#include <thread>  
// 引入C++标准库中的线程支持头文件  
#include <iostream>
#include <cstring>
#include <vector>  
// 引入vector容器，用于存储多个线程

// 处理客户端请求的函数  
void handle_client(int client_fd) {  
    char buffer[1024];  
    // 创建一个buffer，用于接收客户端发送的数据  

    ssize_t len = recv(client_fd, buffer, sizeof(buffer), 0); 
    // 使用recv函数接收客户端发送的数据，并将其存储在buffer中。返回值len为接收到的字节数  

    if (len > 0) {  
        // 如果接收到数据长度大于0，说明客户端发送了数据  

        const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";  
        // 构造一个HTTP响应（模拟一个简单的HTTP响应），包括状态码200和内容“Hello World!”

        send(client_fd, response, strlen(response), 0);  
        // 使用send函数将构造的响应数据发送回客户端  
    }  

    close(client_fd);
}  

// 主函数  
int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};  
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    std::vector<std::thread> workers;  
    // 使用一个vector容器来存储所有创建的线程，便于管理  

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        // 等待并接受客户端连接请求，返回一个新的套接字client_fd，用于与客户端通信  

        workers.emplace_back(handle_client, client_fd);
        // 创建一个新线程，处理当前客户端的请求，线程会调用handle_client函数，并传入client_fd  
        
        workers.back().detach(); // 分离线程  
        // 分离新创建的线程，使得线程能够在后台运行，当线程执行完毕后，自动释放资源  
    }  
    close(server_fd); 
    return 0;  
}  
