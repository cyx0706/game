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
 * 命令 和 数值 必须与 枚举类型 CommandLists 一致
 *
 * commands.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * *
 * -战斗指令
 * attack 1
 * / attack
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
 * @brief 判断命令是否使用错误
 *
 * @param commands 命令行获取的命令
 * @param requiredParamNum 要求的参数个数
 * @return true 则 命令使用错误 false 则 命令使用正确
 */
bool commandWrongUsage(vector<string>& commands, int requiredParamNum)
{
    if (commands.size() != requiredParamNum){
        cout << "wrong usage:" << commands[0] << " needs " << requiredParamNum << " but you have " << commands.size() << endl;

        // 调用帮助命令，显示正确的用法
        Client::commandHelp(commands[0]);

        return true;
    }else{
        return false;
    }
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
 * @brief 分析命令是否可执行
 * 判断: 1. 是否是被禁止的命令 2. 是否是帮助命令 3. 是否是错误命令
 *
 * @param commands 命令行获取的命令(已切片成 vector 类型)
 * @param bannedCommands 被禁止的命令(表示数值)
 * @return true 则为命令基本可以执行，false 则为命令不可执行，需要重新输入
 */
bool Client::analyse(vector<string> commands, vector<int>& bannedCommands){
    // 寻找是否在 commandMap 中有对应的命令
    // 如果没有对应的命令 执行 commandNotFound()
    // commandMap.find() 返回迭代器类型
    auto iter = commandsMap.find(commands[0]);
    if (iter != commandsMap.end()) {
        // 查看是否是帮助命令
        if (commands.size() == 2 && (commands[1] == "-h" || commands[1] == "--help")) {
            commandHelp(commands[0]);

            return false;
        }

        // 帮助命令的相似命令，提示使用正确的命令
        if (commands.size() == 2 && commands[1] == "-help") {
            cout << "use " << commands[0] << " -h or " << commands[0] << " --help" << endl;

            return false;
        }

        // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
        // 表示数值转换为 int 类型
        auto command = (CommandLists)fromString<int>(iter->second);

        // 检查命令是否可用
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
 * @brief 普通状态的执行方法
 * 接受命令 status skills equipment package item equip takeoff discard maps mission
 *         help save
 *
 * @param commands 命令行获取的命令(已切片成 vector 类型)
 * @return true 则命令执行完成 false 则命令未执行，重新输入
 */
bool Client::executeCommand(vector<string> commands, vector<int>& bannedCommands) {
    // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
    // 表示数值转换为 int 类型
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    // status 命令
    if (command == status) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showStatus();
        return false;
    }

    // skills 命令
    if (command == skills) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showSkills();
        return false;
    }

    // equipment 命令
    if (command == equipment) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        auto armor = player.getArmor();
        auto weapon = player.getWeapon();

        if (weapon->id != 0) {
            weapon->showDescription();
        }else {
            cout << "无武器" << endl;
        }

        if (armor->id != 0) {
            armor->showDescription();
        }else {
            cout << "无防具" << endl;
        }

        return false;
    }

    // package 命令
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

    // item 命令
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

    // equip 命令
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

    // takeoff 命令
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

    // discard 命令
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
        cout << "你丢弃了 " << fromString<int>(commands[2]) << "数量的" << commands[1] << endl;

        return false;
    }

    // maps 命令
    if (command == maps) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        mapNow->showDescription();
        return false;
    }

    // mission 命令
    if (command == mission) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        player.showMission();

        return false;
    }

    // help 命令
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

    // save 命令
    if (command == save) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        if (!canSave){
            cout << "command is banned" << endl;
            return false;
        }

        player.save();

        // 商店 信息的保存
        NPC::storeSave();
        // NPC 状态的保存
        NPC::save();

        cout << "game saved" << endl;
        cout << "you can use 'exit' to quit the game" << endl;

        return false;
    }

    return false;
}

/*
 * @brief 战斗状态的执行方法
 * 接受命令 attack skill flee use
 *         help
 *
 * @param commands 命令行获取的命令(已切片成 vector 类型)
 * @param target 战斗的对象
 * @return true 则命令执行完成 false 则命令未执行，重新输入
 */
bool Client::battleExecuteCommand(vector<string> commands, Character &target, vector<int>& bannedCommands) {
    // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
    // 表示数值转换为 int 类型
    auto command = (CommandLists)fromString<int>(commandsMap[commands[0]]);

    // attack 命令
    if (command == attack) {
        // attack 命令只含 1 个参数
        // 若有多个参数 即为 错误的用法
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        // 造成伤害点数值
        int damage;
        // 判断是否暴击
        // Critical 为 int 类型
        // 生成随机数 与 Critical 进行比较
        // 随机数小于 Critical 则为暴击
        //                  否则为正常攻击
        // 伤害数据计算公式
        // player.status.ATK - target.status.DEF
        // 暴击伤害为 player 的 ATK 乘以 1.5
        if (getRandom(1, 100) <= player.status.Critical) {
            damage = int(player.status.ATK * 1.5) - target.status.DEF;
        } else {
            damage = player.status.ATK - target.status.DEF;
        }

        // 伤害低于零值 则伤害为 1
        if (damage <=0) {
            damage = 1;
        }

        cout << player.nameCN << "对" << target.nameCN << "造成了" << damage << "点伤害" << endl;
        target.status.HP -= damage;

        return true;
    }

    // skill 命令
    if (command == skill) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        for (auto& one: player.skills) {
            if (one.nameEN == commands[1]) {
                // MP 不足，直接退出
                if (player.status.MP - one.MP < 0) {
                    cout << "low MP" << endl;
                    return false;
                } else {
                    player.status.MP -= one.MP;
                    cout << "你消耗了" << one.MP << "点 MP" << endl;
                }

                // 攻击的技能
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

                    cout << player.nameCN << "使用" << one.nameCN << "对" << target.nameCN << "造成了" << damage << "点伤害" << endl;
                    target.status.HP -= damage;

                    return true;
                }

                // 加 buff 的技能
                if (one.id[0] == 'B') {
                    player.addBuff(one.buff);
                    cout << "获得" << one.buff.name << "Buff" << endl;

                    return true;
                }

                // 恢复的技能
                if (one.id[0] == 'H') {
                    if (player.status.HP + one.HP > player.maxHP) {
                        cout << "恢复了" << player.maxHP - player.status.HP << "点血量" << endl;
                        player.status.HP = player.maxHP;

                    } else {
                        player.status.HP += one.HP;
                        cout << "恢复了" << one.HP << "点血量" << endl;
                    }
                    return true;
                }

                return false;
            }
        }
        cout << "you do not have the skill " << commands[1] << endl;
        return false;
    }

    // flee 命令
    if (command == flee) {
        if (commandWrongUsage(commands, 1)){
            return false;
        }

        cout << "HaHaHa you can not flee" << endl;
        return false;
    }

    // use 命令
    if (command == use) {
        if (commandWrongUsage(commands, 2)){
            return false;
        }

        if (!player.useDrug(commands[1], target)){
            return false;
        }

        return true;
    }

    // help 命令
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
 * @brief npc交互状态的执行方法
 * 接受命令 purchase sell talk accept_mission finish_mission
 *         help
 *
 * @param commands 命令行获取的命令(已切片成 vector 类型)
 * @param npc 互动的 npc 对象
 * @return true 则命令执行完成 false 则命令未执行，重新输入
 */
bool Client::npcExecuteCommand(vector<string> commands, NPC &npc, vector<int>& bannedCommands) {
    // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
    // 表示数值转换为 int 类型
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

    // help 命令
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
 * @brief 商店交互状态的执行方法
 * 接受命令 purchase sell talk
 *         help
 *
 * @param commands 命令行获取的命令(已切片成 vector 类型)
 * @param 商店 互动的 npc 对象
 * @return true 则命令执行完成 false 则命令未执行，重新输入
 */
bool Client::shopExecuteCommand(vector<string> commands, NPC &npc, vector<int>& bannedCommands) {
    // commandMap 为 map<string, string> 类型，对应 命令 和 表示数值
    // 表示数值转换为 int 类型
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

    // help 命令
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
 * @brief 普通状态的命令行
 */
void Client::base() {
    string str;
    vector<string> commands;
    while (true) {

        // 获得命令行输入
        getline(cin, str);

        // 普通状态可以随时退出命令行
        // 退出命令为 esc quit 任一
        if (str == "esc" || str == "quit") {
            return;
        }

        // exit 退出状态
        if (str == "exit") {
            cout << "Do you want to quit the game? (y/n)";
            string flag;
            cin >> flag;
            if (flag == "y") {
                exit(0);
            }
        }

        // 将命令行获取的命令进行规整
        str = Tool::clean(str);

        // 普通状态禁止 战斗命令 和 与npc交互的命令
        vector<int> bannedCommands = {attack, skill, flee, use,
                                      purchase, sell, talk, accept_mission, finish_mission};

        // 判断命令规整后是否为空
        if (!str.empty()) {
            commands = Tool::split(str, ' ');



            // 分析命令 判断: 1. 是否是被禁止的命令 2. 是否是帮助命令 3. 是否是错误命令
            // 禁止的命令 or 帮助命令 or 错误的命令 则要求重新输入
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // 到此 命令行获取的命令基本可执行
        // 调用 普通状态的执行方法
        // 执行成功则结束命令行
        if (executeCommand(commands, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief 战斗状态的命令行
 */
void Client::base(Character& target) {
    string str;
    vector<string> commands;
    while (true) {

        // 获得命令行输入
        getline(cin, str);

        // 将命令行获取的命令进行规整
        str = Tool::clean(str);

        // 战斗状态禁止 普通命令 、 与npc交互命令 和 保存命令
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      purchase, sell, talk, accept_mission, finish_mission,
                                      save,
        };

        // 判断命令规整后是否为空
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // 分析命令 判断: 1. 是否是被禁止的命令 2. 是否是帮助命令 3. 是否是错误命令
            // 禁止的命令 or 帮助命令 or 错误的命令 则要求重新输入
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // 到此 命令行获取的命令基本可执行
        // 调用 战斗状态的执行方法
        // 执行成功则结束命令行
        if (battleExecuteCommand(commands, target, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief 与NPC互动的命令行
 */
void Client::npcBase(NPC &npc) {
    string str;
    vector<string> commands;
    while (true) {

        // 获得命令行输入
        getline(cin, str);

        // 与NPC互动状态可以随时退出命令行
        // 退出命令为 esc quit exit 任一
        if (str == "esc" || str == "quit" || str == "exit") {
            return;
        }

        // 将命令行获取的命令进行规整
        str = Tool::clean(str);

        // 与npc交互状态禁止  普通命令 、战斗命令 和 保存命令
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      attack, skill, flee, use,
                                      save};

        // 判断命令规整后是否为空
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // 分析命令 判断: 1. 是否是被禁止的命令 2. 是否是帮助命令 3. 是否是错误命令
            // 禁止的命令 or 帮助命令 or 错误的命令 则要求重新输入
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // 到此 命令行获取的命令基本可执行
        // 调用 与NPC互动的执行方法
        // 执行成功则结束命令行
        if (npcExecuteCommand(commands, npc, bannedCommands)) {
            return;
        }
    }
}

/*
 * @brief 与商店互动的命令行
 */
void Client::shopBase(NPC& npc) {
    string str;
    vector<string> commands;
    while (true) {

        // 获得命令行输入
        getline(cin, str);

        // 与商店互动状态可以随时退出命令行
        // 退出命令为 esc quit exit 任一
        if (str == "esc" || str == "quit" || str == "exit") {
            return;
        }

        // 将命令行获取的命令进行规整
        str = Tool::clean(str);

        // 与商店交互状态禁止  普通命令 、战斗命令 和 保存命令
        vector<int> bannedCommands = {status, skills, equipment, package, item, equip, takeoff, discard, maps, mission,
                                      attack, skill, flee, use,
                                      accept_mission, finish_mission,
                                      save};

        // 判断命令规整后是否为空
        if (!str.empty()) {
            commands = Tool::split(str, ' ');

            // 分析命令 判断: 1. 是否是被禁止的命令 2. 是否是帮助命令 3. 是否是错误命令
            // 禁止的命令 or 帮助命令 or 错误的命令 则要求重新输入
            if (!analyse(commands, bannedCommands)) {
                continue;
            }
        } else {
            cout << "empty command" << endl;
            continue;
        }

        // 到此 命令行获取的命令基本可执行
        // 调用 与NPC互动的执行方法
        // 执行成功则结束命令行
        if (shopExecuteCommand(commands, npc, bannedCommands)) {
            return;
        }
    }
}




