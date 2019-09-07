//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>
#include <map>
#include <vector>

#include "Character.h"
#include "global.h"

static const string COMMANDS_TXT_PATH = "../data/commands.txt"; // NOLINT
static const string NAMEID_TXT_PATH = "../data/nameId.txt"; // NOLINT

namespace CMD {
    /*
     * @brief ���е����� �� ��Ӧһ������
     */
    enum CommandLists {
        // ս������
        attack = 1,
        skill = 2,
        flee = 3,
        use = 4,

        // npc��������
        purchase = 5,
        sell = 6,
        talk = 14,
        accept_mission = 19,
        finish_mission = 20,

        // ��ͨ����
        status = 7,
        skills = 21,
        equipment = 8,
        package = 9,
        item = 10,
        equip = 11,
        takeoff = 12,
        discard = 13,
        maps = 15,
        mission = 17,

        // ��������
        help = 16,

        // ��������
        save = 18,
        };
}

using namespace CMD;

/*
 * @brief ��������
 *
 * @note �÷�
 * @code
 * auto client = new Client();
 * client->base();
 * @endcode
 */
class Client {
public:
    Client();

    void base();
    void base(Character& target);
    void npcBase(NPC& npc);
    void shopBase(NPC& npc);

    bool executeCommand(vector<string> commands);
    bool battleExecuteCommand(vector<string> commands, Character& target);
    bool npcExecuteCommand(vector<string> commands, NPC& npc);
    bool shopExecuteCommand(vector<string> commands, NPC& npc);

    bool analyse(vector<string> commands, vector<int>& bannedCommands);

    static bool isBanned(CommandLists command, vector<int>& bannedCommands);
    static void commandHelp(string& command);
private:
    map<string, string> commandsMap;
};

#endif //GAME_CLIENT_H
