//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MISSION_H
#define GAME_MISSION_H

#include "global.h"
class Mission{
public:
    int id;
    string nameEN;
    string nameCN;
    bool isAccepted;
    bool isFinished;
    vector<Item>bonusItem;
    int bonusMoney;
    map<string, int>requiredItem;
    void checkFinished();
};
#endif //GAME_MISSION_H
