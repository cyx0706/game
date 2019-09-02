//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
#include "Map.h"
#include "Tool.h"
#include "Scene.h"
#include <algorithm>
#include <fstream>
#include "Status.h"
#include "Character.h"
#include "global.h"
#include "Client.h"

extern HANDLE hOut;
extern vector<int>commonBannedCommands;
using CMD::CommandLists;
extern Client client;
extern Player player;
extern unique_ptr<Map>mapNow;
//extern vector<NPC>globalNPC;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
//extern vector<Monster>globalMonster;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern SCOORD uPos;
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
                break;
            }
            if(input == SPACE){
                int branch = mapNow->checkEvent();
                if (branch == 2){
                    string barrierType = "item";
                    mapNow->deleteBarrier(uPos, barrierType);
                }
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
    string input;
    while (true){
        int index = 0;
        getline(cin, input);
        string s = input;
        if (!s.empty()){
            // 循环删除所有的空格
            while ((index = s.find(' ', index)) != string::npos){
                s.erase(index); // 删除空格
            }
        }
        if (s == "esc"){
            return; // 返回地图循环
        }
        else{
            client.base(input, commonBannedCommands, 0);
        }
    }
}

// 初始化
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    mapNow = make_unique<Map>();
    mapNow->load(1);
}
// 游戏的主循环
//void GameLoop::loop() {
//    initGame();
//    while (TRUE){
//        mapLoop();
//        //commandLoop
//    }
//
//}
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
    if (character.status.Speed > player.status.Speed){
        playerTurn = false;
        cout << "敌人的先制攻击, 造成了" << character.status.ATK - player.status.DEF << "点伤害" << endl;
        if(player.isDead()){
            // 死亡场景
            return;
        }
        else{
            cout << "玩家剩余血量:" << player.status.HP << endl;
        }
    }
    int turn = 0; // 回合数
    while (true){
        turn += 1;
        cout << "回合:" << turn << endl;
        // 判断buff是否过期
        for (auto iter = player.buffs.begin(); iter != player.buffs.end() ; iter++) {
            // 只有为0才过期
            if ((*iter).duration != 0){
                (*iter).duration--;
            }
            else{
                player.buffs.erase(iter);
            }
        }
        // 前置判断使回合可以跳过
        // 玩家回合
        if (playerTurn){
            cout << "玩家的回合:你的行动" << endl;
            //TODO::调用command分析

            // 下一次是敌人行动
            playerTurn = false;
            enemyTurn = true;

            // 击败敌人
            if (character.isDead()){
                cout << "你击败了" << character.nameCN << "获得了胜利";
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
                break;
            }
            else{
                cout << character.nameCN << "剩余HP: " <<character.status.HP << endl;
            }
        }
        if (enemyTurn){
            int possible = getRandom(1, 100); // 暴击概率
            int fluctuation = getRandom(90, 110); // 伤害的波动
            int damage = int(character.status.ATK * fluctuation / 100);
            // 暴击
            if (possible >= character.status.Critical){
                damage = int(damage * 1.5);
            }
            player.status.HP -= (damage - player.status.DEF);
            if (player.isDead()){
                // 调用死亡场景
                break;
            }
            else{
                cout << player.nameCN << "剩余HP: " << player.status.HP << endl;
            }
            // 下一次是玩家行动
            playerTurn = true;
            enemyTurn = false;
        }
    }
}




