cmake_minimum_required(VERSION 3.10)
project(TinyWebServer)

# 设置C++标准为C++14
set(CMAKE_CXX_STANDARD 14)  # 设置为C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 生成可执行文件
add_executable(server 
    # src/http_parser.cpp
    # src/multi_thread_server.cpp
    # src/socket_server.cpp
    # src/thread_pool.hpp  # 注意头文件一般不用添加到这里
    src/epoll_server.cpp
    # src/Thread_epoll.cpp
)
# 链接系统库（Linux必需）
target_link_libraries(server pthread)

