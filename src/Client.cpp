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
#include "templateHeader.h"
#include "global.h"

extern Player player;

/*
 * @brief �������Ҳ�����Ӧ����ʱ����� command not found
 */
void commandNotFound(){
    cout << "command not found" << endl;
}

/*
 * @brief ���ڵ�ǰ����������ֹʱ����� command is banned
 */
void commandIsBanned() {
    cout << "command is banned" << endl;
}

/*
 * @brief ���� ����������� map
 * ���ļ� commands.txt ��ȡ ���� �� ��ʾ��ֵ������commandsMap��
 * ���� �� ��ֵ ������ ö������ CommandLists һ��
 *
 * commands.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * *
 * -ս��ָ��
 * attack 1
 * / attack
 * // ��ս��״̬��ʹ����ͨ���� ����ָ��Ŀ��
 *
 * ��'-'��ͷΪ   �������
 * ��'/ '��ͷΪ  �����÷�
 * ��'// '��ͷΪ ������
 *
 * ��ͷû�����η���Ϊ����
 * �������һ���� �ÿո����
 * * * * * * * * * * * * * * * * * * * * * * * *
 */
Client::Client() {
    // ��ִ���ļ������� build �ｫ command.txt ���ڸ�Ŀ¼/data��
    ifstream f(COMMANDS_TXT_PATH);

    string str;
    // һ�ζ�ȡһ��
    while(getline(f,str))
    {
        // ���С���ͷΪ'/'����ͷΪ'-'���� �޳�
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            // ȡ���� ���� �� ��ʾ��ֵ
            vector<string> keyValue = Tool::split(str);
            // ���� �� ��ʾ��ֵ ���� map ��
            this->commandsMap.insert(pair<string, string>(keyValue[0], keyValue[1]));
        }
    }

    f.close();
}

/*
 * @brief �������İ������
 *
 * @param command Ҫ�鿴����������
 */
void Client::commandHelp(string& command) {
    ifstream f(COMMANDS_TXT_PATH);

    string str;
    // һ�ζ�ȡһ�� ����Ҫ���ҵ������λ��
    while (getline(f,str)) {
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            str = Tool::split(str)[0];

            if (command == str) {
                break;
            }
        }
    }

    // �����ļ���β��Ϊû�д�������İ���
    if (f.eof()) {
        cout << "no command help" << endl;
    } else {
        while (getline(f,str)) {
            // �ļ��� ÿ������(����:��������÷���������)֮���ÿ��и���
            // �������������������а�����伴Ϊ������
            if (str.empty()) {
                break;
            }
            else {
                // ���ļ��д洢���������������ݽ��и�ʽ�滻
                // ��'/ ' ��ͷ��Ϊ�����÷�
                // ��'// '��ͷ��Ϊ������
                regex informationPattern("// ");
                regex usagePattern("/ ");

                // �����ȶ�'// '����ƥ�䣬��Ϊ'/ 'Ϊ'// '��һ����
                str = regex_replace(str, informationPattern, "���:");
                str = regex_replace(str, usagePattern, "�÷�:");

                cout << str << endl;
            }
        }
    }

    f.close();
}

/*
 * @brief ��������Ƿ񱻽�ֹ
 *
 * @param command ���������������(��ʾ��ֵ)
 * @param bannedCommands ����ֹ���õ�����
 * @return δ����ֹ�򷵻�false ����ֹ�򷵻�true
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
 * @brief ���������Ƿ��ִ��
 * �ж�: 1. �Ƿ��Ǳ���ֹ������ 2. �Ƿ��ǰ������� 3. �Ƿ��Ǵ�������
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @param bannedCommands ����ֹ������(��ʾ��ֵ)
 * @return true ��Ϊ�����������ִ�У�false ��Ϊ�����ִ�У���������
 */
bool Client::analyse(vector<string> commands, vector<int>& bannedCommands){
    // Ѱ���Ƿ��� commandMap ���ж�Ӧ������
    // ���û�ж�Ӧ������ ִ�� commandNotFound()
    // commandMap.find() ���ص���������
    auto iter = commandsMap.find(commands[0]);
    if (iter != commandsMap.end()) {
        // �鿴�Ƿ��ǰ�������
        if (commands.size() == 2 && (commands[1] == "-h" || commands[1] == "--help")) {
            commandHelp(commands[0]);

            return false;
        }

        // ������������������ʾʹ����ȷ������
        if (commands.size() == 2 && commands[1] == "-help") {
            cout << "use " << commands[0] << " -h or " << commands[0] << " --help" << endl;

            return false;
        }

        // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
        // ��ʾ��ֵת��Ϊ int ����
        auto command = (CommandLists)fromString<int>(iter->second);

        // ��������Ƿ����
        if (!isBanned(command, bannedCommands)) {
            return true;
        } else {
            commandIsBanned();

            return false;
        }
    } else {
        commandNotFound();

        return false;
    }
}

/*
 * @brief ��ͨ״̬������ִ��
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::executeCommand(vector<string> commands) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    switch (command) {
        case purchase:
            if (commands.size() != 2 || commands.size() != 4) {
                cout << "wrong usage" << endl;
                return false;
            }


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
        case maps:
            break;
        case help:
            break;
        case mission:
            break;
        case save:
            break;
        default:
            return false;
    }
}

/*
 * @brief ��ͨ״̬��������
 */
void Client::base() {
    string str;
    vector<string> commands;

    while (true) {
        getline(cin, str);
        if (str == "esc") {
            return;
        }

        str = Tool::clean(str);

        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            vector<int> bannedCommands = {attack, skill, flee, use};

            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        }
        else {
            cout << "empty command" << endl;
            continue;
        }

        if (executeCommand(commands)) {
            return;
        }
    }
}

/*
 * @brief ս��״̬������ִ��
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @param target ս���Ķ���
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::battleExecuteCommand(vector<string> commands, Character &target) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    if (command == attack) {
        // attack ����ֻ�� 1 ������
        // ���ж������ ��Ϊ ������÷�
        if (commands.size() != 1) {
            cout << "wrong usage" << endl;
            return false;
        }

        // ����˺�����ֵ
        int damage;
        // �ж��Ƿ񱩻�
        // Critical Ϊ int ����
        // ��������� �� Critical ���бȽ�
        // �����С�� Critical ��Ϊ����
        //                  ����Ϊ��������
        // �˺����ݼ��㹫ʽ
        // player.status.ATK - target.status.DEF
        // �����˺�Ϊ player �� ATK ���� 1.5
        if (getRandom(1, 100) <= player.status.Critical) {
            damage = int(player.status.ATK * 1.5) - target.status.DEF;
        } else {
            damage= player.status.ATK - target.status.DEF;
        }

        // �˺�������ֵ ���˺�Ϊ 1
        if (damage <=0) {
            damage = 1;
        }

        cout << player.nameCN << "��" << target.nameCN << "�����" << damage << "���˺�" << endl;
        target.status.HP -= damage;

        return true;
    }

    if (command == skill) {
        if (commands.size() != 2) {
            cout << "wrong usage" << endl;
            return false;
        }

        for (auto& one: player.skills) {
            if (one.nameEN == commands[1]) {
                // MP ���㣬ֱ���˳�
                if (player.status.MP + one.MP < 0) {
                    cout << "low MP" << endl;
                    return false;
                }

                // �����ļ���
                if (one.id[0] == 'A') {
                    int damage;
                    if (getRandom(1, 100) <= player.status.Critical) {
                        damage = int((player.status.ATK + one.ATK )* 1.5) - target.status.DEF;
                    } else {
                        damage= player.status.ATK + one.ATK - target.status.DEF;
                    }

                    if (damage <= 0) {
                        damage = 1;
                    }

                    cout << player.nameCN << "ʹ��" << one.nameCN << "��" << target.nameCN << "�����" << damage << "���˺�" << endl;
                    target.status.HP -= damage;

                    return true;
                }

                // �� buff �ļ���
                if (one.id[0] == 'B') {
                    player.addBuff(one.buff);
                    cout << "���" << one.buff.name << "Buff" << endl;

                    return true;
                }

                // �ָ��ļ���
                if (one.id[0] == 'H') {
                    if (player.status.HP + one.HP > player.maxHP) {
                        cout << "�ָ���" << player.maxHP - player.status.HP << "��Ѫ��" << endl;
                        player.status.HP = player.maxHP;

                    } else {
                        player.status.HP += one.HP;
                        cout << "�ָ���" << one.HP << "��Ѫ��" << endl;
                    }
                    return true;
                }

                return false;
            }
        }
        cout << "you do not have the skill " << commands[1] << endl;
        return false;
    }

    if (command == flee) {
        if (commands.size() != 1) {
            cout << "wrong usage" << endl;
            return false;
        }

        cout << "HaHaHa you can not flee" << endl;
        return false;
    }

    if (command == use) {
        if (commands.size() != 2) {
            cout << "wrong usage" << endl;
            return false;
        }

        if (!player.useDrug(commands[1], target)){
            return false;
        }

        return true;
    }
}

/*
 * @brief �����е� ��� ���� ����
 *
 * @param str �����һ������
 * @param bannedCommands ����ֹ������
 * @param index ������������Ӧ������λ��
 */
void Client::base(Character& target) {
    string str;
    vector<string> commands;

    while (true) {
        getline(cin, str);

        str = Tool::clean(str);

        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            vector<int> bannedCommands = {purchase, sell,
                                          status, equipment, package, item, equip, takeoff, discard, talk,
                                          maps,
                                          mission,
                                          save};

            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        }
        else {
            cout << "empty command" << endl;
            continue;
        }

        if (battleExecuteCommand(commands, target)) {
            return;
        }
    }
}

bool Client::shopExecuteCommand(vector<string> commands, NPC &npc) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);
    if (command == purchase) {
        if (commands.size() != 2 || commands.size() != 4) {
            cout << "wrong usage" << endl;
            return false;
        }

        if (commands.size() == 4 && commands[2] != "-number") {
            cout << "wrong usage" << endl;
            return false;
        }

        ifstream f(NAMEID_TXT_PATH);
        map<string, string>data = Tool::dataMap(f);

        auto iter = data.find(commands[1]);
        if (iter == data.end()) {
            cout << "no such item" << endl;
            return false;
        }

        int itemId = fromString<int>(data[commands[1]]);

        if (commands.size() == 2){
            npc.buy(itemId, 1, player);
        } else{
            // TODO
            npc.buy(itemId, fromString<int>(commands[3]), player);
        }
        return true;
    }

    if (command == sell) {
        if (commands.size()) {
            int itemId;
            if (itemId < 100) {
                Weapon thing(itemId);
            }

            if (itemId < 200) {
                Armor thing(itemId);
            }

            if (itemId < 300) {
                Drug thing(itemId);
            }

            if (itemId < 400) {
                Item thing(itemId);
            }
        }
    }
    return false;
}


void Client::npcBase(NPC &npc) {
    string str;
    vector<string> commands;

    while (true) {
        getline(cin, str);

        str = Tool::clean(str);

        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            vector<int> bannedCommands = {attack, skill, flee, use,
                                          save};

            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        if (shopExecuteCommand(commands, npc)) {
            return;
        }
    }
}


