//
// Created by cyx on 2019/8/27.
//

#ifndef GAME_GLOBAL_H
#define GAME_GLOBAL_H

#include "NPC.h"
#include "Player.h"

#include "Client.h"
/*
 * 全局变量存到这里
 */
string mapPath = R"(../data/map.txt)";
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
vector<NPC>npcs;
Player player;

COORD uPos = {1, 1};


Client client;
#endif //GAME_GLOBAL_H
