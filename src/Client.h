//
// Created by Leo on 2019-08-26.
//

#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H
#include "global.h"
#include <string>
#include <map>
#include <vector>

static const string COMMANDS_TXT_PATH = "../data/commands.txt"; // NOLINT

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
 * @brief 命令行类
 *
 * @note 用法
 * @code
 * auto client = new Client();
 *
 * string str;
 * getline(cin, str);
 *
 * vector<string> bannedCommands;
 * // 添加 禁止命令到 bannedCommands 中，若没有为空
 *
 * while(true) {
 *     client->base(str, bannedCommands);
 *
 *     getline(cin, str);
 *
 *     // 添加 循环结束条件
 * }
 * @endcode
 */
class Client {
public:
    Client();
    void base(string str, vector<int>& bannedCommands);
    void executeCommand(vector<string> commands,  vector<int>& bannedCommands);
    static bool isBanned(CommandLists command, vector<int>& bannedCommands);
    static void commandHelp(string& command);
private:
    map<string, string> commandsMap;
};


#endif //TEST_CLIENT_H
