//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
#include "Map.h"
#include "Tool.h"
#include <algorithm>
/*
 * @brief 游戏的地图循环
 * 需要初始化完成后才能调用
 */


extern HANDLE hOut;
extern vector<int>commonBannedCommands;
extern enum CommandLists;
extern Client client;
extern Player player;
extern vector<NPC>globalNPC;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
extern vector<Monster>globalMonster;
extern CONSOLE_CURSOR_INFO cursorInfo;

void GameLoop::mapLoop() {
    mapNow->initMap();
    mapNow->print();
    char input;
    while(true)
    {
        if(kbhit())
        {
            mapNow->clean(uPos);//清除原有输出
            input = getchar();
            // 输入Esc转换为命令行模式,返回true通知主调函数
            if (input == ESC){
                system("cls"); // 清空屏幕
                commandLoop();
                break;
            }
            if(input == SPACE){
                mapNow->checkEvent();
                mapNow->print();
            }
            else{
                mapNow->move(input);
                // 移动到下一章地图
                if (mapNow->checkTopMapTransition()||mapNow->checkBottomMapTransition()){
                    system("cls");
                    cout << "Waiting......";
                    system("cls");
                    // 生成指针并读取数据
                    mapNow = mapNow->nextMap(mapNow->roadTo[uPos]);
                    // 绘制地图
                    mapNow->initMap();
                    mapNow->checkSpecialScene();
                }
                mapNow->print();
            }
        }
    }
}

/*
 * @brief 命令行循环
 * 输入esc命令切换,在mapLoop里被调用
 */
void commandLoop(){
    string input;
    while (TRUE){
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
            client.base(input, commonBannedCommands);
        }


    }
}
// 初始化
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
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
    initGame();
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
            // 调用场景
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
 *
 * @param 战斗对象id的引用
 */
void GameLoop::battleLoop(const string& charId, bool isMonster) {
    Character *character = nullptr;
    bool playerFirst = true;
    if (isMonster){
        //读取怪物信息
        Monster monster;
        // 需要强制转换
        character = reinterpret_cast<Character *>(&monster);

    }
    else{
        //不需要再读取了
        for (auto iter = globalNPC.begin(); iter != globalNPC.end() ; iter++) {
            if ((*iter).id == charId){
                // 为啥基类指针不能指向派生类
                character = reinterpret_cast<Character *>(&(*iter));
                break;
            }
        }
    }
    if (character->status.Speed > player.status.Speed){
        playerFirst = false;
    }
    while (true){
        if (playerFirst){
            //调用command分析

        }
        else{
            int possible = getRandom(1, 100);
            // 暴击
            if (possible >= character->status.Critical){

            }
        }
        if (player.isDead()){
            // 调用死亡场景
            break;
        }
        if (character->isDead()){
            cout << "你击败了" << character->nameCN << "获得了胜利";
            cout << "获得经验" << character->fallingExp << endl;
            //  cout << "获得金钱" << character
            // 判断掉落物品
            // 升级判定
             if(isMonster){
                 auto temp = dynamic_cast<Monster*>(character);
                 for (auto iter = temp->fallingItem.begin(); iter != temp->fallingItem.end() ; iter++) {
                     cout << "获得" << (*iter).nameCN << endl;
                     // 接口了?
                     player.addItem(*iter);
                     // 任务怪属性设定

                 }
             }
             else{
                 // 场景
             }

        }
    }

}