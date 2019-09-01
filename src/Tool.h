//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_TOOL_H
#define GAME_TOOL_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include "global.h"

/*
 * @brief 工具类
 *
 */
class Tool {
public:
    static vector<string> split(const string& str, char sep = ' ');
    static string clean(const string& str);
    static map<string, string> dataMap(ifstream& f);
    static bool boolFromString(string& str);
};

int getRandom(int min, int max);
#endif //GAME_TOOL_H
