//
// Created by Leo on 2019-08-26.
//

#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "Tool.h"
#include "global.h"

/*
 * @brief ����min��max֮ǰ�������
 */
int getRandom(int min, int max){
    return (rand() % static_cast<int>(max + 1 - min) + min);
}

/*
 * @brief ���ָ�����Ƭ�ַ���������vector<string>
 *
 * @param str ��������ַ���
 * @param sep �ָ���
 * @return ���ش������ַ���
 */
vector<string> Tool::split(const string& str, const char sep){
    vector<string> splitStr;
    int length = str.length();

    // ��һ�����Ƿָ��� sep �ַ���λ��
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
            // �� index ����ʼ ���� i - index ���ȸ��ַ� �����ַ���
            splitStr.push_back(str.substr(index, i - index));
            index = i + 1;
        }

        i++;

        // ���һ���ַ��������� vector ��
        if (i == length && index < i){
            splitStr.push_back(str.substr(index, i - index));
        }
    }

    return splitStr;
}

/*
 * @brief �����ַ�������ǰ��ո�ȥ���������ո��Ϊ�����ո�
 *
 * @param str ��������ַ���
 * @return ���ش������ַ���
 */
string Tool::clean(const string& str) {
    string cleanedStr = str;

    // �޳��ַ���ǰ���Ŀո�
    for (int i = 0; i < (int)cleanedStr.length(); i++){
        if (cleanedStr[i] != ' '){
            cleanedStr = cleanedStr.substr(i, (int)cleanedStr.length() - i + 1);
            break;
        }
    }

    // �޳��ַ���β���Ŀո�
    for (int i = (int)cleanedStr.length() - 1; i >= 0; i--){
        if (cleanedStr[i] != ' '){
            cleanedStr = cleanedStr.substr(0, i + 1);
            break;
        }
    }

    // �޳��м䲿�ֵ������ո�
    for (int i = 0; i < (int)cleanedStr.length() - 1; i++){
        // �ҵ��м䲿�ֵ��׸��ո��λ��
        if (cleanedStr[i] == ' '){
            // �ҵ��׸��ո���׸����ǿո��λ��
            int end = i;
            while (cleanedStr[end] == ' '){
                end++;
            }

            // �ж��Ƿ��ǵ����ո�
            if (end - i != 1){
                cleanedStr =  cleanedStr.substr(0, i + 1) + cleanedStr.substr(end);
            }
        }
    }

    return cleanedStr;
}

/*
 * @brief ���ļ��ж�ȡ��ֵ�ԣ���ȡ�����н���
 *
 * @param f �ļ��� Ҫ��ȡ���ļ����ֵĿ�ʼ
 * @return ���ش��ļ��ж�ȡ�����ļ�ֵ�� map
 */
map<string, string> Tool::dataMap(ifstream& f) {
    map<string, string> data;
    string str;

    while (getline(f, str)) {
        // �������н���
        if (!str.empty()) {
            vector<string> keyValue = Tool::split(str);

            data.insert(pair<string, string>(keyValue[0], keyValue[1]));
        } else {
            break;
        }
    }

    return data;
}

/*
 * @brief �����ı�������Ӧ�Ĳ���ֵ
 *
 * @param str �ַ��� "true" "false"
 * @return "true" ���ز���ֵ true ���򷵻� false
 */
bool Tool::boolFromString(string &str) {
    return str == "true";
}

/*
 * @brief ���ַ���ת��Ϊ�ַ�����
 * ��Ҫע���ַ�����Ҫ����50�ĳ���
 */
void Tool::stringToChar(string &str, char t[]) {
    int j = 0;
    for (auto iter = str.begin(); iter != str.end(); iter++) {
        t[j++] = *iter;
    }
    t[j++] = '\0';
}
