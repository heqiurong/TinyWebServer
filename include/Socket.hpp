/*
    * Socket.hpp：socket封装类头文件
    * 作者：何秋蓉
    * 描述：封装socket相关操作，提供简单接口
    * 创建时间：2025年3月
*/
#pragma once // 防止头文件重复包含
#include <sys/socket.h> // socket系统调用

class Socket {
public:
    Socket();           // 构造函数
    ~Socket();          // 析构函数（关键点：RAII释放资源）
    
    // 创建socket描述符（关键点：IPv4 + TCP协议）
    bool create();
    
    // 绑定端口（关键点：htonl/htons处理字节序）
    bool bind(int port);
    
    // 监听连接（关键点：backlog参数含义）
    bool listen(int backlog = 10);
    
    // 获取socket文件描述符
    int getFd() const { return sock_fd_; }

private:
    int sock_fd_ = -1;  // socket文件描述符（初始化为无效值）
};
