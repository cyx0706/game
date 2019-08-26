//
// Created by Leo on 2019-08-26.
//

#include "Tool.h"

#include <vector>

using namespace std;

/*
 * @brief 按分隔符切片字符串，返回vector<string>
 *
 * @param str 待处理的字符串
 * @param sep 分隔符
 * @return 返回处理后的字符串
 */
vector<string> Tool::split(const string& str, const char sep){
    vector<string> splitStr;
    int length = str.length();

    // 第一个不是分隔符 sep 字符的位置
    int start = 0;
    for (int i = 0; i < length; i++){
        if (str[i] != sep){
            start = i;
            break;
        }
    }

    int index = start;
    for (int i = index; i < length; ){
        if (str[i] == sep){
            // 从 index 处开始 拷贝 i - index 长度个字符 返回字符串
            splitStr.push_back(str.substr(index, i - index));
            index = i + 1;
        }

        i++;

        // 最后一个字符串，推入 vector 中
        if (i == length && index < i){
            splitStr.push_back(str.substr(index, i - index));
        }
    }

    return splitStr;
}

/*
 * @brief 处理字符串，将前后空格去除，连续空格变为单个空格
 *
 * @param str 待处理的字符串
 * @return 返回处理后的字符串
 */
string Tool::clean(const string& str) {
    string cleanedStr = str;

    // 剔除字符串前部的空格
    for (int i = 0; i < (int)cleanedStr.length(); i++){
        if (cleanedStr[i] != ' '){
            cleanedStr = cleanedStr.substr(i, (int)cleanedStr.length() - i + 1);
            break;
        }
    }

    // 剔除字符串尾部的空格
    for (int i = (int)cleanedStr.length() - 1; i >= 0; i--){
        if (cleanedStr[i] != ' '){
            cleanedStr = cleanedStr.substr(0, i + 1);
            break;
        }
    }

    // 剔除中间部分的连续空格
    for (int i = 0; i < (int)cleanedStr.length() - 1; i++){
        // 找到中间部分的首个空格的位置
        if (cleanedStr[i] == ' '){
            // 找到首个空格后首个不是空格的位置
            int end = i;
            while (cleanedStr[end] == ' '){
                end++;
            }

            // 判断是否是单个空格
            if (end - i != 1){
                cleanedStr =  cleanedStr.substr(0, i + 1) + cleanedStr.substr(end);
            }
        }
    }

    return cleanedStr;
}
