//
// Created by Leo on 2019-08-26.
//

#include "Client.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

#include "Tool.h"
#include "global.h"

/*
 * @brief 用于在找不到对应命令时，输出 command not found
 */
void commandNotFound(){
    cout << "command not found" << endl;
}

/*
 * @brief 用于当前输入的命令被禁止时，输出 command is banned
 */
void commandIsBanned() {
    cout << "command is banned" << endl;
}

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
 * 以'-'开头为   命令分类
 * 以'/ '开头为  命令用法
 * 以'// '开头为 命令简介
 *
 * 开头没有修饰符的为命令
 * 命令后有一数字 用空格隔开
 * * * * * * * * * * * * * * * * * * * * * * * *
 */
Client::Client() {
    // 可执行文件生成在 build 里将 command.txt 放在根目录/data下
    ifstream f(COMMANDS_TXT_PATH);

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

/*
 * @brief 输出命令的帮助语句
 *
 * @param command 要查看帮助的命令
 */
void Client::commandHelp(string& command) {
    ifstream f(COMMANDS_TXT_PATH);

    string str;
    // 一次读取一行 读到要查找到命令的位置
    while (getline(f,str)) {
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            str = Tool::split(str)[0];

            if (command == str) {
                break;
            }
        }
    }

    // 读到文件结尾即为没有此条命令的帮助
    if (f.eof()) {
        cout << "no command help" << endl;
    } else {
        while (getline(f,str)) {
            // 文件中 每条命令(包含:命令、命令用法、命令简介)之间用空行隔开
            // 读到空行则此命令的所有帮助语句即为输出完毕
            if (str.empty()) {
                break;
            }
            else {
                // 将文件中存储此条命令的相关内容进行格式替换
                // 以'/ ' 开头的为命令用法
                // 以'// '开头的为命令简介
                regex informationPattern("// ");
                regex usagePattern("/ ");

                // 必须先对'// '进行匹配，因为'/ '为'// '的一部分
                str = regex_replace(str, informationPattern, "简介:");
                str = regex_replace(str, usagePattern, "用法:");

                cout << str << endl;
            }
        }
    }

    f.close();
}

/*
 * @brief 检查命令是否被禁止
 *
 * @param command 命令行输入的命令(表示数值)
 * @param bannedCommands 被禁止调用的命令
 * @return 未被禁止则返回false 被禁止则返回true
 */
bool Client::isBanned(CommandLists command, vector<int> &bannedCommands) {
    for (int one: bannedCommands) {
        if ((CommandLists)one == command) {
            return true;
        }
    }

    return false;
}

/*
 * @brief 搜寻对应命令 可用即执行
 *
 * @param commands 命令行输入的命令
 */
void Client::executeCommand(vector<string> commands,  vector<int>& bannedCommands, int index) {
    // 寻找是否在 commandMap 中有对应的命令
    // 如果没有对应的命令 执行 commandNotFound()
    // commandMap.find() 返回迭代器类型
    auto iter = commandsMap.find(commands[0]);
    if (iter != commandsMap.end()) {
        // 查看是否是帮助命令
        if (commands.size() == 2 && (commands[1] == "-h" || commands[1] == "--help")) {
            commandHelp(commands[0]);

            return;
        }

        // 帮助命令的相似命令，提示使用正确的命令
        if (commands.size() == 2 && commands[1] == "-help") {
            cout << "use " << commands[0] << " -h or " << commands[0] << " --help" << endl;

            return;
        }

        // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
        // 表示数值转换为 int 类型
        stringstream stream;
        int number = 0;
        string text(iter->second);
        stream << text;
        stream >> number ;
        stream.clear();

        auto command = (CommandLists)number;

        // 检查命令是否可用
        if (!isBanned(command, bannedCommands)) {
            // TODO
            switch (command) {
                case attack:
                    break;
                case skill:
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
                default:
                    commandNotFound();
            }
        } else {
            commandIsBanned();
        }
    } else {
        commandNotFound();
    }
}

/*
 * @brief 命令行的 输出 接受 处理
 *
 * @param str 输入的一行命令
 * @param bannedCommands 被禁止的命令
 * @param index 命令操作对象对应的索引位置
 */
void Client::base(string str, vector<int>& bannedCommands, int index) {
        // 将前后空格去除，连续空格变为单个空格
        str = Tool::clean(str);

        if (!str.empty()) {
            vector commands = Tool::split(str, ' ');

            // 调用命令行输入命令对应的函数
            executeCommand(commands, bannedCommands, index);
        } else {
            cout << "empty command" << endl;
        }
}

