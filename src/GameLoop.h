//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_GAMELOOP_H
#define GAME_GAMELOOP_H


#include "global.h"
#include "Character.h"
#define SPACE 32
#define ESC 27

//÷¥––”Œœ∑
class GameLoop{
public:
    static void mapLoop();
    static void initGame();
    static void newGame();
    static void npcLoop(NPC &talkedNPC);
    static void commandLoop();
    static void gameInterface();
    static void battleLoop(Character &character);
    static void shopLoop(NPC &npc);
};

#endif //GAME_GAMELOOP_H
