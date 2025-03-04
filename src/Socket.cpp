/*
    Socket类的实现
    1. 创建socket
    2. 绑定socket
    3. 监听socket
    4. 关闭socket
    创建时间：2025年3月3日
*/
#include "Socket.hpp"
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <sys/socket.h> 
#include <unistd.h>     // close函数
#include <cstring>      // memset
#include <stdexcept>    // 异常处理

Socket::Socket(): sock_fd_(-1) {
    // 构造函数暂时留空，由create()方法初始化sock_fd_
}

Socket::~Socket() {
    if (sock_fd_ != -1) {
        close(sock_fd_); // 确保关闭socket（关键点：RAII核心）
        sock_fd_ = -1;
    }
}

bool Socket::create() {
    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd_ == -1) {
        throw std::runtime_error("socket创建失败: " + std::string(strerror(errno)));
    }
    return true;
}

bool Socket::bind(int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有网卡
    addr.sin_port = htons(port);              // 端口转为网络字节序
    
    if (::bind(sock_fd_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("bind失败: " + std::string(strerror(errno)));
    }
    return true;
}

bool Socket::listen(int backlog) {
    if (::listen(sock_fd_, backlog) == -1) {
        throw std::runtime_error("listen失败: " + std::string(strerror(errno)));
    }
    return true;
}
