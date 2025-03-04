/*
    * http_request.hpp
    * Header file for HttpRequest structure
    * 创建时间：2025年3月
*/
#include <string>               // 引入标准库中的string头文件，用于处理字符串
#include <unordered_map>        // 引入unordered_map头文件，用于处理哈希表（无序映射）

// 定义HttpRequest结构体，用于表示一个HTTP请求
struct HttpRequest {
    std::string method;         // HTTP请求方法（例如：GET、POST）
    std::string path;           // 请求的路径（例如：/index.html）
    std::string version;        // HTTP版本（例如：HTTP/1.1）
    // 存储HTTP请求头，使用哈希表（键值对）
    std::unordered_map<std::string, std::string> headers;
};