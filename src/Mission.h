//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MISSION_H
#define GAME_MISSION_H

#include <string>

#include "Item.h"
#include "global.h"

static const string MISSION_TXT_PATH = "../data/mission"; // NOLINT

/*
 * @brief »ŒŒÒ¿‡
 */
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
    bool isProcess;
    int bonusMoney;
    int bonusExperiencePoint;
    string assigner;
    map<int, int> requiredItem;
};
#endif //GAME_MISSION_H
