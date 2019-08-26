//
// Created by Leo on 2019-08-26.
//

#ifndef TEST_TOOL_H
#define TEST_TOOL_H

#include <vector>
#include <string>

using namespace std;
/*
 * @brief 工具类
 *
 */
class Tool {
public:
    static vector<string> split(const string& str, char sep = ' ');
    static string clean(const string& str);
};


#endif //TEST_TOOL_H
