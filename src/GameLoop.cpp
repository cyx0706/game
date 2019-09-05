//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
#include "Map.h"
#include "Tool.h"
#include "Scene.h"
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "Status.h"
#include "Character.h"
#include "global.h"
#include "Client.h"

extern HANDLE hOut;
using CMD::CommandLists;
extern Client client;
extern Player player;
extern unique_ptr<Map>mapNow;
extern vector<NPC>globalNPC;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern SCOORD uPos;
extern vector<string>npcName;
/*
 * @brief 游戏的地图循环
 * 需要初始化完成后才能调用
 */
void GameLoop::mapLoop() {
    GetConsoleCursorInfo(hOut, &cursorInfo); //获取控制台光标信息
    cursorInfo.bVisible = false;             //隐藏控制台光标
    SetConsoleCursorInfo(hOut, &cursorInfo); //设置控制台光标状态
    mapNow->initMap();
    mapNow->print();
    char input;
    while(true){
        if(kbhit()){
            mapNow->clean(uPos);//清除原有输出
            input = getch();
            // 输入Esc转换为命令行模式,返回true通知主调函数
            if (input == ESC){
                system("cls"); // 清空屏幕
                commandLoop();
                mapNow->initPos = uPos;
                mapNow->initMap();
            }
            if(input == SPACE){
                int branch = mapNow->checkEvent();
                if (branch == 2){
                    string barrierType = "item";
                    mapNow->deleteBarrier(uPos, barrierType);
                }
                if (branch == 0){
                    string barrierType = "monster";
//                    mapNow->deleteBarrier(uPos, barrierType);
                }
                mapNow->clean(uPos);
                mapNow->print();
            }
            else{
                mapNow->move(input);
                mapNow->print();
            }
        }
    }
}

/*
 * @brief 命令行循环
 * 输入esc命令切换,在mapLoop里被调用
 */
void GameLoop::commandLoop() {
    player.playerMenu();
    Map::setCursorStatus(true);
    client.base();
    Map::setCursorStatus(false);
    system("cls");
}

// 初始化
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    NPC::readLastLine = 0; // 初始化
    mapNow = make_unique<Map>();
    mapNow->load(1);
    Skill skill1("A01");
    player.skills.push_back(skill1);
    Skill skill2("B01");
    player.skills.push_back(skill2);
    Skill skill3("H01");
    player.skills.push_back(skill3);
    Skill skill4("H02");
    player.skills.push_back(skill4);
    player.maxMP = 100;
    player.maxHP = 150;
    player.addItem(201, 2);
    for (unsigned int i = 0; i < npcName.size(); i++) {
        NPC aNPC(npcName[i]);
        globalNPC.push_back(aNPC);
    }

}



void GameLoop::gameInterface(){
    string title = "暴咕攻城狮的异世界狂想曲";
    auto x = short(screenInfo.dwSize.X - title.length() / 2);
    COORD pos = {x, 5};
    SetConsoleCursorPosition(hOut, pos);
    cout << "+----------------------------+"
         << "|  暴咕 攻城狮 的异世界狂想曲  |"
         << "+----------------------------+"
         << endl;
    cout << "NewGame      Load      Exit   " << endl;
}

void GameLoop::gameStart() {
    // 展示游戏的界面
    gameInterface();
    string input;
    while (true){
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::tolower); // 转成小写
        // 删除首尾空格
        if( !input.empty() ){
            input.erase(0,input.find_first_not_of(' '));
            input.erase(input.find_last_not_of(' ') + 1);
        }

        if (input == "newgame"){
            // 提示输入用户的名字
            cout << "请输入你的用户名:\b\b\b\b\b";
            cin >> input;
            player.nameCN = input;
            player.nameEN = "None";

            Scene scene(1);
            ifstream fp;
            fp.open("../src/scene1.txt");
            fp >> scene; // 展示剧情
            mapLoop();
        }
        else if (input == "load"){
            // 调用读取
            mapLoop();
        }
        else if (input == "exit"){
            cout << "Bye!";
            Sleep(1000); // 睡1s
            exit(1);
        }
        else{
            // 提示信息
            cout << "newgame: 新的游戏" << endl;
            cout << "load: 读取存档" << endl;
            cout << "exit: 退出" << endl;
        }
    }

}

void GameLoop::npcLoop(NPC &talkedNPC) {
    system("cls");
    Map::setCursorStatus(true);
    talkedNPC.NPCMenu(player);
    client.npcBase(talkedNPC);
    system("cls");
}
/*
 * @brief 战斗循环直到一方死亡结束
 * 函数负责战斗的逻辑处理
 * 玩家的行动由命令系统给出接口实现
 * 函数判断buff的持续而buff的加成计算由命令系统实现
 * 玩家死亡循环结束，调用死亡场景
 * 敌对方死亡结算奖励
 * @param 战斗对象character
 */
void GameLoop::battleLoop(Character &character) {
    bool playerTurn = true;
    bool enemyTurn = false;
    int damage;
    SCOORD curPos;
    system("cls");

    cout << "遭遇怪物" << endl;
    if (character.status.Speed > player.status.Speed){
        playerTurn = true;
        enemyTurn = false;
        damage = character.status.ATK - player.status.DEF;
        if (damage <= 0){
            damage = 1;
        }
        cout << "敌人的先制攻击, 造成了" << damage << "点伤害" << endl;
        player.status.HP -= damage;
        if(player.isDead()){
            player.deadScene();
            return;
        }
        else{
            cout << "玩家剩余血量:" << player.status.HP << endl;
        }
    }
    system("pause");
    system("cls");

    int turn = 0; // 回合数
    while (true){
        turn += 1;

        // 战斗界面
        for (int i = 0; i < 80; i++) {
            cout << "*";
        }
        cout << endl;
        for (short k = 1; k < 20; k++) {
            curPos = {40, k};
            Map::gotoxy(curPos);
            cout << "*";
        }
        curPos = {0, 20};
        Map::gotoxy(curPos);
        for (int i = 0; i < 80; i++) {
            cout << "*";
        }
        // 信息输出
        curPos = {7,1};
        Map::gotoxy(curPos);
        cout << "敌人:" << character.nameCN << "("  << character.nameEN << ")";
        curPos.Y = 2;
        Map::gotoxy(curPos);
        cout << "-> HP:" << character.status.HP;
        curPos.Y = 3;
        Map::gotoxy(curPos);
        cout << "-> MP:" << character.status.MP;
        curPos.Y = 4;
        Map::gotoxy(curPos);
        cout << "-> ATK:" << character.status.ATK;
        curPos.Y = 5;
        Map::gotoxy(curPos);
        cout << "-> DEF:" << character.status.DEF;
        curPos.Y = 6;
        Map::gotoxy(curPos);
        cout << "--------------------" << endl;
        curPos.Y = 7;
        Map::gotoxy(curPos);
        cout << "玩家: " << player.nameCN << endl;
        curPos.X = 7;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "BUFF:";
        curPos.Y++;
        Map::gotoxy(curPos);
        for (auto iter = player.buffs.begin(); iter != player.buffs.end();) {
            // 只有为0才过期
            if ((*iter).duration != 0){
                (*iter).duration--;
                if (iter->duration == 0){
                    player.deleteBuff((*iter));
                    player.buffs.erase(iter);
                    continue;
                }
                else{
                    cout << iter->name << "\t剩余" << iter->duration << "回合";
                    curPos.Y++;
                    Map::gotoxy(curPos);
                }
            }
            iter++;
        }
        cout << "-> HP:" << player.status.HP;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> MP:" << player.status.MP;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> ATK:" << player.status.ATK;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> DEF:" << player.status.DEF;

        curPos = {50, 1};
        Map::gotoxy(curPos);
        cout << "\t\t玩家技能";
        short y = 2;
        if (!player.skills.empty()){
            for (auto j = player.skills.begin(); j <player.skills.end() ; j++) {
                curPos = {48, y++};
                Map::gotoxy(curPos);
                cout << setiosflags(ios_base::left)
                     << setw(16) << j->nameCN
                     << setw(16) << j->nameEN
                     << "消耗MP:" << j->MP;
            }
        }
        else{
            curPos = {48, y++};
            cout << "\t\t无技能";
        }

        curPos.Y += 2;
        Map::gotoxy(curPos);
        cout << "\t\t玩家物品";
        curPos.Y++;
        player.battleBagShow(curPos);

        curPos = {0, 21};
        Map::gotoxy(curPos);
        cout << "-------------------" << endl;
        cout << "回合:" << turn << endl;
        cout << "-------------------" << endl;

        // 前置判断使回合可以跳过
        // 玩家回合
        if (playerTurn){
            cout << "玩家的回合:你的行动" << endl;
            Map::setCursorStatus(true);
            client.base(character);
            // 下一次是敌人行动
            playerTurn = false;
            enemyTurn = true;

            // 击败敌人
            if (character.isDead()){
                cout << "你击败了" << character.nameCN << "获得了胜利" << endl;
                cout << "获得经验" << character.fallingExp << endl;
                cout << "获得金钱" << character.fallingMoney << endl;
                player.addExp(character.fallingExp);
                player.addMoney(character.fallingMoney);
                // 判断掉落物品
                auto *monster = dynamic_cast<Monster*>(&character);
                // 转换失败时为空指针
                if(monster){
                   for(auto iter = monster->fallingItem.begin(); iter!=monster->fallingItem.end(); iter++){
                       cout << "怪物掉落了" << (*iter).nameCN << endl;
                       player.addItem((*iter).id, (*iter).num);
                   }
                }
                system("pause");
                system("cls");
                break;
            }
            if (player.isDead()){
                player.deadScene();
                system("pause");
                exit(1);
            }
        }
        if (enemyTurn){
            int possible = getRandom(1, 100); // 暴击概率
            int fluctuation = getRandom(90, 110); // 伤害的波动
            damage = int(character.status.ATK * fluctuation / 100);
            // 暴击
            if (possible >= character.status.Critical){
                damage = int(damage * 1.5);
            }
            damage -= player.status.DEF;
            if (damage <= 0){
                damage = 1;
            }
            player.status.HP -= damage;
            cout << character.nameCN << "对你造成" << damage << "伤害" << endl;
            if (player.isDead()){
                player.deadScene();
                system("pause");
                exit(1);
            }
            // 下一次是玩家行动
            playerTurn = true;
            enemyTurn = false;
        }
        system("pause");
        Map::setCursorStatus(false);
        system("cls");
    }
}




