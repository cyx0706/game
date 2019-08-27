//
// Created by Leo on 2019-08-26.
//

#include "Client.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include<sstream>

#include "Tool.h"

using namespace std;

/*
 * @brief 所有的命令 并 对应一个数字
 */
enum CommandLists {
    attack = 1,
    skill,
    flee,
    purchase,
    sell,
    status,
    equipment,
    package,
    item,
    equip,
    takeoff,
    discard,
    talk,
    fight,
    slaughter,
    maps,
    help,
    mission,
    save,
};

/*
 * @brief 生成 命令行命令的 map
 * 从文件 commands.txt 读取 命令 和 表示数值，存入commandsMap中
 * 命令 和 数值 必须与上方 枚举类型 CommandLists 一致
 *
 * commands.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * *
 * -战斗指令
 * attack 1
 * / attack <character_name>
 * // （战斗状态）使用普通攻击 攻击指定目标
 *
 * 以'-'开头为  命令分类
 * 以'/'开头为  命令格式
 * 以'//'开头为 命令介绍
 *
 * 开头没有修饰符的为命令
 * 命令后有一数字 用空格隔开
 * * * * * * * * * * * * * * * * * * * * * * * *
 */
Client::Client() {
    // 可执行文件生成在 build 里将 command.txt 放在根目录/data下
    ifstream f("../data/commands.txt");

    string str;
    // 一次读取一行
    while(getline(f,str))
    {
        // 空行、开头为'/'、开头为'-'的行 剔除
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            // 取出出 命令 和 表示数值
            vector<string> keyValue = Tool::split(str);
            // 命令 和 表示数值 存入 map 中
            this->commandsMap.insert(pair<string, string>(keyValue[0], keyValue[1]));
        }
    }

    f.close();
}

/* 临时测试函数 ********************************************************/
void sayHello() {
    cout << "Hello Hello Hello" << endl;
}

void sayNo() {
    cout << "NO NO NO" << endl;
}
/* 临时测试函数 ********************************************************/

/*
 * @brief 用于在找不到对应命令时，输出 command not found
 */
void commandNotFound(){
    cout << "command not found" << endl;
}

/*
 * @brief 搜寻对应命令 返回函数指针
 *
 * @param commands 命令行输入的命令
 * @return 返回函数指针
 */
auto Client::findCommand(vector<string> commands) {
    // 寻找是否在 commandMap 中有对应的命令
    // 如果没有对应的命令返回 commandNotFound() 函数指针
    // commandMap.find() 返回迭代器类型
    auto iter = commandsMap.find(commands[0]);
    if (iter != commandsMap.end()){

        // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
        // 表示数值转换为 int 类型
        stringstream stream;
        int number = 0;
        string text(iter->second);
        stream << text;
        stream >> number ;
        stream.clear();

        // 找到对应的函数，返回函数指针
        // TODO
        auto command = (CommandLists)number;
        switch (command) {
            case attack:
                return sayHello;
                break;
            case skill:
                return sayNo;
                break;
            case flee:
                break;
            case purchase:
                break;
            case sell:
                break;
            case status:
                break;
            case equipment:
                break;
            case package:
                break;
            case item:
                break;
            case equip:
                break;
            case takeoff:
                break;
            case discard:
                break;
            case talk:
                break;
            case fight:
                break;
            case slaughter:
                break;
            case maps:
                break;
            case help:
                break;
            case mission:
                break;
            case save:
                exit(1);
                break;
        }
    } else {
        return commandNotFound;
    }
}

/*
 * @brief 命令行的 输出 接受
 *
 * @param str
 */
void Client::base(string str) {
        // 将前后空格去除，连续空格变为单个空格
        str = Tool::clean(str);

        if (!str.empty()) {
            vector commands = Tool::split(str, ' ');

            // 调用命令行输入命令对应的函数
            auto fun = findCommand(commands);
            fun();
        }else {
            cout << "empty command" << endl;
        }
}

