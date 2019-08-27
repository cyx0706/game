//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_GAMELOOP_H
#define GAME_GAMELOOP_H

#include "Map.h"
#define SPACE 32
#define ESC 27
using namespace std;
//执行游戏
class GameLoop{
public:
    friend class Map;
    friend class Monster;
    friend class Player;
    friend class NPC;
    void mapLoop();
    void initGame();
    void gameStart();
    void loop();
    static COORD uPos; // 玩家位置
};
// 游戏的地图循环
void GameLoop::mapLoop() {
    char input;
    while(TRUE)
    {
        if(kbhit())
        {
            clean(*uPos);//清除原有输出
            input = getch();
            // 输入Esc转换为命令行模式,返回true通知主调函数
            if (input == ESC){
                //commandLoop
                break;
            }
            if(input == SPACE){
                checkEvent(uPos);
                print(uPos);
            }
            else{
                move(&charPos, input, edgeLeft, edgeRight, barrier);
                // 移动到下一章地图
                if (checkTopMapTransition(uPos)||checkBottomMapTransition(uPos)){
                    system("cls")
                    cout << "Waiting......";
                    system("cls")
                    // 生成指针并读取数据
                    unique_ptr<Map>newMap = nextMap(roadTo[uPos]);
                    // 绘制地图
                    newMap.initMap();
                }
                print(charPos);
            }
        }
    }
}

// 初始化
void GameLoop::initGame() {

}
// 游戏的主循环
void GameLoop::loop() {
    initGame();
    while (TRUE){
        mapLoop();
        //commandLoop
    }

}
void GameLoop::gameStart() {

}
#endif //GAME_GAMELOOP_H
