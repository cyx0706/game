//
// Created by Leo on 2019-08-26.
//

#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <string>
#include <map>
#include <vector>

#include "global.h"

static const string COMMANDS_TXT_PATH = "../data/commands.txt"; // NOLINT

namespace CMD {
    /*
     * @brief ���е����� �� ��Ӧһ������
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
}

using namespace CMD;

/*
 * @brief ��������
 *
 * @note �÷�
 * @code
 * auto client = new Client();
 *
 * string str;
 * getline(cin, str);
 *
 * vector<string> bannedCommands;
 * // ��� ��ֹ��� bannedCommands �У���û��Ϊ��
 *
 * while(true) {
 *     client->base(str, bannedCommands);
 *
 *     getline(cin, str);
 *
 *     // ��� ѭ����������
 * }
 * @endcode
 */
class Client {
public:
    Client();
    void base(string str, vector<int>& bannedCommands, int index);
    void executeCommand(vector<string> commands,  vector<int>& bannedCommands, int index);
    static bool isBanned(CommandLists command, vector<int>& bannedCommands);
    static void commandHelp(string& command);
private:
    map<string, string> commandsMap;
};

#endif //TEST_CLIENT_H
