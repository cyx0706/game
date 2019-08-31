//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MISSION_H
#define GAME_MISSION_H

#include <string>

#include "Item.h"
#include "global.h"

static const string MISSION_TXT_PATH = "../data/mission"; // NOLINT

class Mission {
public:
    explicit Mission(int id);
    void checkFinished();
public:
    int id;
    string nameEN;
    string nameCN;
    bool isAccepted;
    bool isFinished;
    // vector<Item> bonusItem;
    int bonusMoney;
    map<int, int> requiredItem;
};
#endif //GAME_MISSION_H
