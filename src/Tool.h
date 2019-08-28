//
// Created by Leo on 2019-08-26.
//

#ifndef TEST_TOOL_H
#define TEST_TOOL_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "global.h"
/*
 * @brief 工具类
 *
 */
class Tool {
public:
    static vector<string> split(const string& str, char sep = ' ');
    static string clean(const string& str);
};

template <class T>
static string toString(const T &v){
    ostringstream os; // 创建字符串输出流
    os << v; // 将变量写入字符串流
    return os.str(); // 返回输出流生成的字符串
}

template <class T>
inline T fromString(const string &str){
    istringstream is(str); // 创建字符串输入流
    T v;
    is >> v; // 从字符串输入流中读取变量v
    return v;
}
#endif //TEST_TOOL_H
