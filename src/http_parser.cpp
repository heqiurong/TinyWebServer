/*
    * http_parser.cpp: 实现HTTP请求解析函数
    * 解析HTTP请求，提取请求方法、路径、版本、请求头等信息
    * 作者：heqiurong
    * 创建时间：2025年3月
*/
#include <sstream>               // 引入stringstream头文件，用于字符串流操作
#include <unordered_map>         // 引入unordered_map头文件，用于存储HTTP头部的键值对
#include "http_request.hpp"      // 引入http_request.hpp头文件，包含HttpRequest结构体的定义
#include <iostream>              // 引入iostream头文件，用于标准输入输出

// 解析HTTP请求
bool parse_http_request(const char* request, HttpRequest& req) {
    // 使用istringstream将请求字符串转换为流，便于逐行读取
    std::istringstream iss(request);
    // 定义一个字符串变量，用于存储每一行内容
    std::string line;
    // 读取第一行，也就是请求行（如"GET /index.html HTTP/1.1"）
    std::getline(iss, line);
    // 使用istringstream将请求行作为流进行处理
    std::istringstream line_stream(line);
     // 从流中读取请求方法、路径和版本，并赋值给HttpRequest结构体的成员
    line_stream >> req.method >> req.path >> req.version;

    // 解析请求头部 Host: localhost\r\n\r\n
    // 逐行读取请求头部，直到遇到空行（请求头的结束标志）
    while (std::getline(iss, line) && line != "\r") {  
        // 查找冒号的位置，分隔键和值
        size_t colon_pos = line.find(':');  
        // 如果找到了冒号
        if (colon_pos != std::string::npos) {  
            // 提取冒号前面的部分作为键
            std::string key = line.substr(0, colon_pos);  
            // 提取冒号后面的部分作为值，并去掉末尾的回车符和换行符（一般HTTP头部的值会有"\r\n"结尾）
            std::string value = line.substr(colon_pos + 2, line.size() - colon_pos - 3);
            // 将键值对存入HttpRequest的headers成员（一个unordered_map）
            req.headers[key] = value;  
        }
    }
    return true;
}

/*
    测试代码:通过
*/
// int main(){
//     const char* request = "GET /index.html HTTP/1.1\r\nHost: localhost\r\nres: hello world!\r\nchinese: 帅哥来了\r\n\r\n";
//     HttpRequest req;
//     parse_http_request(request, req);
//     std::cout << "Method: " << req.method << std::endl;
//     std::cout << "Path: " << req.path << std::endl;
//     std::cout << "Version: " << req.version << std::endl;
//     std::cout << "Host: " << req.headers["Host"] << std::endl;
//     std::cout << "Content-Length: " << req.headers["Content-Length"] << std::endl;
//     std::cout << "User-Agent: " << req.headers["User-Agent"] << std::endl;
//     std::cout << "res: " << req.headers["res"] << std::endl;
//     std::cout << "chinese: " << req.headers["chinese"] << std::endl;
//     return 0;
// }