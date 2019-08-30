//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_GAMELOOP_H
#define GAME_GAMELOOP_H


#include "global.h"
#define SPACE 32
#define ESC 27

//执行游戏
class GameLoop{
public:
    static void mapLoop();
    static void initGame();
    static void gameStart();
//    static void loop();
    static void commandLoop();
    static void gameInterface();
    static void battleLoop(const string& charId, bool isMonster = true);
};

#endif //GAME_GAMELOOP_H
