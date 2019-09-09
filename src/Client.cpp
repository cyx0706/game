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
#include "Map.h"
#include "templateHeader.h"
#include "Character.h"
#include "global.h"

extern Player player;
extern unique_ptr<Map> mapNow;
extern vector<NPC> globalNPC;
extern bool canSave;

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
 * @brief �ж������Ƿ�ʹ�ô���
 *
 * @param commands �����л�ȡ������
 * @param requiredParamNum Ҫ��Ĳ�������
 * @return true �� ����ʹ�ô��� false �� ����ʹ����ȷ
 */
bool commandWrongUsage(vector<string>& commands, int requiredParamNum)
{
    if (commands.size() != requiredParamNum){
        cout << "wrong usage:" << commands[0] << " needs " << requiredParamNum << " but you have " << commands.size() << endl;

        // ���ð��������ʾ��ȷ���÷�
        Client::commandHelp(commands[0]);

        return true;
    }else{
        return false;
    }
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
 * @return true ��Ϊ�����������ִ�У�false ��Ϊ�����ִ�У���Ҫ��������
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
 * @brief ��ͨ״̬��ִ�з���
 * �������� status skills equipment package item equip takeoff discard maps mission
 *         help save
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::executeCommand(vector<string> commands, vector<int>& bannedCommands) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    // status ����
    if (command == status) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showStatus();
        return false;
    }

    // skills ����
    if (command == skills) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showSkills();
        return false;
    }

    // equipment ����
    if (command == equipment) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        auto armor = player.getArmor();
        auto weapon = player.getWeapon();

        if (weapon->id != 0) {
            weapon->showDescription();
        }else {
            cout << "������" << endl;
        }

        if (armor->id != 0) {
            armor->showDescription();
        }else {
            cout << "�޷���" << endl;
        }

        return false;
    }

    // package ����
    if (command == package) {
        if (commands.size() == 1){
            cout << "use package -w to show your weapons" << endl;
            cout << "use package -a to show your armors" << endl;
            cout << "use package -d to show your drugs" << endl;
            cout << "use package -i to show your items" << endl;

            return false;
        }


        if (commandWrongUsage(commands, 2)){
            return false;
        }

        if (commands[1] != "-w" || commands[1] != "-a" || commands[1] != "-d" || commands[1] != "-i"){
            cout << "wrong usage" << endl;
            return false;
        }

        if (commands[1] == "-w") {
            player.showWeapons();
        }
        if (commands[1] == "-a") {
            player.showArmors();
        }
        if (commands[1] == "-d") {
            player.showDrugs();
        }
        if (commands[1] == "-i") {
            player.showItems();
        }

        return false;
    }

    // item ����
    if (command == item) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        ifstream f(NAMEID_TXT_PATH);
        map<string, string>data = Tool::dataMap(f);

        auto iter = data.find(commands[1]);
        if (iter == data.end()) {
            cout << "no such item" << endl;
            return false;
        }

        if (!player.showItem(fromString<int>(data[commands[1]]))) {
            cout << "no such item" << endl;
        }
        return false;
    }

    // equip ����
    if (command == equip) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        ifstream f(NAMEID_TXT_PATH);
        map<string, string>data = Tool::dataMap(f);

        auto iter = data.find(commands[1]);
        if (iter == data.end()) {
            cout << "no such equipment" << endl;
            return false;
        }

        if (fromString<int>(data[commands[1]]) < 100) {
            player.equipWeapon(commands[1]);
        } else {
            player.equipArmor(commands[1]);
        }

        return false;
    }

    // takeoff ����
    if (command == takeoff) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        ifstream f(NAMEID_TXT_PATH);
        map<string, string>data = Tool::dataMap(f);

        auto iter = data.find(commands[1]);
        if (iter == data.end()) {
            cout << "no such equipment" << endl;
            return false;
        }

        if (fromString<int>(data[commands[1]]) < 100) {
            player.takeOffWeapon(commands[1]);
        } else {
            player.takeOffArmor(commands[1]);
        }

        return false;
    }

    // discard ����
    if (command == discard) {
        if (commandWrongUsage(commands, 3)){
            return false;
        }

        if (commands[2] == "0") {
            return false;
        }

        ifstream f(NAMEID_TXT_PATH);
        map<string, string>data = Tool::dataMap(f);

        auto iter = data.find(commands[1]);
        if (iter == data.end()) {
            cout << "no such equipment" << endl;
            return false;
        }

        if (player.getItem(fromString<int>(data[commands[1]])) < fromString<int>(commands[2])) {
            cout << "you do not have " << fromString<int>(commands[2]) << " but " << fromString<int>(data[commands[1]]) << endl;
            return false;
        }

        player.deleteItem(fromString<int>(data[commands[1]]), fromString<int>(commands[2]));
        cout << "�㶪���� " << fromString<int>(commands[2]) << "������" << commands[1] << endl;

        return false;
    }

    // maps ����
    if (command == maps) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        mapNow->showDescription();
        return false;
    }

    // mission ����
    if (command == mission) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showMission();

        return false;
    }

    // help ����
    if (command == help) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "these commands are available" << endl;
        for (const auto& one: this->commandsMap) {
            if (find(bannedCommands.begin(), bannedCommands.end(), fromString<int>(one.second)) == bannedCommands.end()){
                cout << one.first << endl;
            }
        }

        cout << "you can use" << endl;
        cout <<"'command' + -h or 'command' + --help" << endl;
        cout << "look for specific help" << endl;
        return false;
    }

    // save ����
    if (command == save) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        if (!canSave){
            cout << "command is banned" << endl;
            return false;
        }

        player.save();

        // �̵� ��Ϣ�ı���
        NPC::storeSave();
        // NPC ״̬�ı���
        NPC::save();

        cout << "game saved" << endl;
        cout << "you can use 'exit' to quit the game" << endl;

        return false;
    }

    return false;
}

/*
 * @brief ս��״̬��ִ�з���
 * �������� attack skill flee use
 *         help
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @param target ս���Ķ���
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::battleExecuteCommand(vector<string> commands, Character &target, vector<int>& bannedCommands) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    // attack ����
    if (command == attack) {
        // attack ����ֻ�� 1 ������
        // ���ж������ ��Ϊ ������÷�
        if (commandWrongUsage(commands, 1)){
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
            damage = player.status.ATK - target.status.DEF;
        }

        // �˺�������ֵ ���˺�Ϊ 1
        if (damage <=0) {
            damage = 1;
        }

        cout << player.nameCN << "��" << target.nameCN << "�����" << damage << "���˺�" << endl;
        target.status.HP -= damage;

        return true;
    }

    // skill ����
    if (command == skill) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        for (auto& one: player.skills) {
            if (one.nameEN == commands[1]) {
                // MP ���㣬ֱ���˳�
                if (player.status.MP - one.MP < 0) {
                    cout << "low MP" << endl;
                    return false;
                } else {
                    player.status.MP -= one.MP;
                    cout << "��������" << one.MP << "�� MP" << endl;
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

    // flee ����
    if (command == flee) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "HaHaHa you can not flee" << endl;
        return false;
    }

    // use ����
    if (command == use) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        if (!player.useDrug(commands[1], target)){
            return false;
        }

        return true;
    }

    // help ����
    if (command == help) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "these commands are available" << endl;
        for (const auto& one: this->commandsMap) {
            if (find(bannedCommands.begin(), bannedCommands.end(), fromString<int>(one.second)) == bannedCommands.end()){
                cout << one.first << endl;
            }
        }

        cout << "you can use" << endl;
        cout <<"'command' + -h or 'command' + --help" << endl;
        cout << "look for specific help" << endl;
        return false;
    }

    return false;
}

/*
 * @brief npc����״̬��ִ�з���
 * �������� purchase sell talk accept_mission finish_mission
 *         help
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @param npc ������ npc ����
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::npcExecuteCommand(vector<string> commands, NPC &npc, vector<int>& bannedCommands) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);
    if (command == purchase) {
        if (commands.size() == 2) {
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
                npc.buy(itemId, fromString<int>(commands[3]), player);
            }
            return false;
        }

        if (commands.size() == 4 && commands[2] == "-number")
        {
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
                npc.buy(itemId, fromString<int>(commands[3]), player);
            }
            return false;
        }

        cout << "wrong usage" << endl;
        return false;
    }

    if (command == sell) {
        if (commandWrongUsage(commands, 3)){
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

        if (itemId < 100) {
            Weapon one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }

        if (itemId < 200) {
            Armor one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }

        if (itemId < 300) {
            Drug one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        } else{
            Item one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }
    }

    if (command == talk) {
        npc.talk(player);

        return false;
    }

    if (command == accept_mission) {
        npc.assignQuest(player);

        return false;
    }

    if (command == finish_mission) {
        npc.finishQuest(player);

        return false;
    }

    // help ����
    if (command == help) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "these commands are available" << endl;
        for (const auto& one: this->commandsMap) {
            if (find(bannedCommands.begin(), bannedCommands.end(), fromString<int>(one.second)) == bannedCommands.end()){
                cout << one.first << endl;
            }
        }

        cout << "you can use" << endl;
        cout <<"'command' + -h or 'command' + --help" << endl;
        cout << "look for specific help" << endl;
        return false;
    }

    return false;
}

/*
 * @brief �̵꽻��״̬��ִ�з���
 * �������� purchase sell talk
 *         help
 *
 * @param commands �����л�ȡ������(����Ƭ�� vector ����)
 * @param �̵� ������ npc ����
 * @return true ������ִ����� false ������δִ�У���������
 */
bool Client::shopExecuteCommand(vector<string> commands, NPC &npc, vector<int>& bannedCommands) {
    // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
    // ��ʾ��ֵת��Ϊ int ����
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);
    if (command == purchase) {
        if (commands.size() == 2) {
            ifstream f(NAMEID_TXT_PATH);
            map<string, string>data = Tool::dataMap(f);

            auto iter = data.find(commands[1]);
            if (iter == data.end()) {
                cout << "no such item" << endl;
                return false;
            }

            int itemId = fromString<int>(data[commands[1]]);

            if (commands.size() == 2){
                if (npc.buy(itemId, 1, player)){
                    system("cls");
                    npc.shopMenu(player);
                }
            } else{
                if (npc.buy(itemId, fromString<int>(commands[3]), player)){
                    system("cls");
                    npc.shopMenu(player);
                }
            }
            return false;
        }

        if (commands.size() == 4 && commands[2] == "-number")
        {
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
                npc.buy(itemId, fromString<int>(commands[3]), player);
            }
            return false;
        }

        cout << "wrong usage" << endl;
        return false;
    }

    if (command == sell) {
        if (commandWrongUsage(commands, 3)){
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

        if (itemId < 100) {
            Weapon one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }

        if (itemId < 200) {
            Armor one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }

        if (itemId < 300) {
            Drug one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        } else{
            Item one(itemId);
            npc.sell(one, fromString<int>(commands[2]), player);
            return false;
        }
    }

    if (command == talk) {
        npc.talk(player);

        return false;
    }

    // help ����
    if (command == help) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "these commands are available" << endl;
        for (const auto& one: this->commandsMap) {
            if (find(bannedCommands.begin(), bannedCommands.end(), fromString<int>(one.second)) == bannedCommands.end()){
                cout << one.first << endl;
            }
        }

        cout << "you can use" << endl;
        cout <<"'command' + -h or 'command' + --help" << endl;
        cout << "look for specific help" << endl;
        return false;
    }

    return false;
}

/*
 * @brief ��ͨ״̬��������
 */
void Client::base() {
    string str;
    vector<string> commands;
    while (true) {

        // �������������
        getline(cin, str);

        // ��ͨ״̬������ʱ�˳�������
        // �˳�����Ϊ esc quit ��һ
        if (str == "esc" || str == "quit") {
            return;
        }

        // exit �˳�״̬
        if (str == "exit") {
            cout << "Do you want to quit the game? (y/n)";
            string flag;
            cin >> flag;
            if (flag == "y") {
                exit(0);
            }
        }

        // �������л�ȡ��������й���
        str = Tool::clean(str);

        // ��ͨ״̬��ֹ ս������ �� ��npc����������
        vector<int> bannedCommands = {attack, skill, flee, use,
                                      purchase, sell, talk, accept_mission, finish_mission};

        // �ж�����������Ƿ�Ϊ��
        if (!str.empty()) {
            commands = Tool::split(str, ' ');



            // �������� �ж�: 1. �Ƿ��Ǳ���ֹ������ 2. �Ƿ��ǰ������� 3. �Ƿ��Ǵ�������
            // ��ֹ������ or �������� or ��������� ��Ҫ����������
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // ���� �����л�ȡ�����������ִ��
        // ���� ��ͨ״̬��ִ�з���
        // ִ�гɹ������������
        if (executeCommand(commands, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief ս��״̬��������
 */
void Client::base(Character& target) {
    string str;
    vector<string> commands;
    while (true) {

        // �������������
        getline(cin, str);

        // �������л�ȡ��������й���
        str = Tool::clean(str);

        // ս��״̬��ֹ ��ͨ���� �� ��npc�������� �� ��������
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      purchase, sell, talk, accept_mission, finish_mission,
                                      save,
        };

        // �ж�����������Ƿ�Ϊ��
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // �������� �ж�: 1. �Ƿ��Ǳ���ֹ������ 2. �Ƿ��ǰ������� 3. �Ƿ��Ǵ�������
            // ��ֹ������ or �������� or ��������� ��Ҫ����������
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // ���� �����л�ȡ�����������ִ��
        // ���� ս��״̬��ִ�з���
        // ִ�гɹ������������
        if (battleExecuteCommand(commands, target, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief ��NPC������������
 */
void Client::npcBase(NPC &npc) {
    string str;
    vector<string> commands;
    while (true) {

        // �������������
        getline(cin, str);

        // ��NPC����״̬������ʱ�˳�������
        // �˳�����Ϊ esc quit exit ��һ
        if (str == "esc" || str == "quit" || str == "exit") {
            return;
        }

        // �������л�ȡ��������й���
        str = Tool::clean(str);

        // ��npc����״̬��ֹ  ��ͨ���� ��ս������ �� ��������
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      attack, skill, flee, use,
                                      save};

        // �ж�����������Ƿ�Ϊ��
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // �������� �ж�: 1. �Ƿ��Ǳ���ֹ������ 2. �Ƿ��ǰ������� 3. �Ƿ��Ǵ�������
            // ��ֹ������ or �������� or ��������� ��Ҫ����������
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // ���� �����л�ȡ�����������ִ��
        // ���� ��NPC������ִ�з���
        // ִ�гɹ������������
        if (npcExecuteCommand(commands, npc, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief ���̵껥����������
 */
void Client::shopBase(NPC& npc) {
    string str;
    vector<string> commands;
    while (true) {

        // �������������
        getline(cin, str);

        // ���̵껥��״̬������ʱ�˳�������
        // �˳�����Ϊ esc quit exit ��һ
        if (str == "esc" || str == "quit" || str == "exit") {
            return;
        }

        // �������л�ȡ��������й���
        str = Tool::clean(str);

        // ���̵꽻��״̬��ֹ  ��ͨ���� ��ս������ �� ��������
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      attack, skill, flee, use,
                                      accept_mission, finish_mission,
                                      save};

        // �ж�����������Ƿ�Ϊ��
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // �������� �ж�: 1. �Ƿ��Ǳ���ֹ������ 2. �Ƿ��ǰ������� 3. �Ƿ��Ǵ�������
            // ��ֹ������ or �������� or ��������� ��Ҫ����������
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // ���� �����л�ȡ�����������ִ��
        // ���� ��NPC������ִ�з���
        // ִ�гɹ������������
        if (shopExecuteCommand(commands, npc, bannedCommands)) {
            return;
        }
    }
}




