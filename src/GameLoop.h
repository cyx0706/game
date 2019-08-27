//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_GAMELOOP_H
#define GAME_GAMELOOP_H

#include "Map.h"
#include "global.h"
#define SPACE 32
#define ESC 27

unique_ptr<Map>mapNow;
extern COORD uPos;
extern enum CommandLists;

using namespace std;

//执行游戏
class GameLoop{
public:
    void mapLoop();
    void initGame();
    void gameStart();
    void loop();
    void commandLoop();
};

#endif //GAME_GAMELOOP_H
