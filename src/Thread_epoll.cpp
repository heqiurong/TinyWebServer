/*
    epoll结合线程池的服务器
    创建人：何秋蓉
    创建时间：2025年3月3日
*/
#include <iostream>
#include <vector>
#include "thread_pool.hpp"
#include <sys/epoll.h>
#include "Socket.cpp"
#include <netinet/in.h>
#include <unistd.h>

#define MAX_EVENTS 1024

int main(){
    try{
        Socket server_socket;
        if(!server_socket.create()){
            throw std::runtime_error("Socket创建失败");
        }
        if(!server_socket.bind(8080)){
            throw std::runtime_error("绑定失败");
        }
        if(!server_socket.listen(5)){
            throw std::runtime_error("监听失败");
        }

        std::cout<<"服务器启动，监听端口：8080"<<std::endl;

        ThreadPool pool(4); // 创建线程池，线程数为4

        int epoll_fd = epoll_create1(0);
        epoll_event ev;
        std::vector<epoll_event> events(MAX_EVENTS);
        ev.events = EPOLLIN;
        ev.data.fd = server_socket.getFd();

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket.getFd(), &ev);

        while(true){
            int num_fds = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
            if(num_fds == -1){
                throw std::runtime_error("Epoll等待失败");
                return -1;
            }else{
                std::cout<<"Epoll等待成功"<<std::endl;
            }
            // 遍历所有发生的事件
            int server_socket_fd = server_socket.getFd();
            for(int i = 0; i < num_fds; ++i){
                int fd = events[i].data.fd;  // 获取文件描述符
                
                if (fd == server_socket.getFd()) {  // 如果事件是服务器 Socket（表示有新连接）
                    // 处理新连接
                    int client_fd = accept(server_socket.getFd(), nullptr, nullptr);
                    if (client_fd == -1) {
                        std::cerr << "接受连接失败\n";
                        continue;
                    }else{
                        std::cout<<"接受连接成功"<<std::endl;
                    }
                
                    ev.events = EPOLLIN | EPOLLET;  // 设置为可读事件，并启用边缘触发（EPOLLET）
                    ev.data.fd = client_fd;  // 设置客户端文件描述符
                
                    // 将客户端 Socket 加入 epoll 监听
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                        std::cerr << "客户端连接注册失败\n";
                        continue;
                    }else{
                        std::cout<<"客户端连接成功"<<std::endl;
                    }
                } else {  // 如果事件是客户端 Socket（表示客户端发送数据）
                    // 处理客户端请求
                    char buffer[1024];
                    ssize_t len = recv(fd, buffer, sizeof(buffer), 0);
                    if (len > 0) {
                        send(fd, "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello Epoll!", 46, 0);
                    }
                    close(fd);  // 确保连接关闭
                }
            }
        }
    }catch(const std::exception& e){
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}
