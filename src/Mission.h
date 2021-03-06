//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MISSION_H
#define GAME_MISSION_H

#include <string>

#include "Item.h"
#include "Character.h"
#include "global.h"
static const string MISSION_TXT_PATH = "../data/mission.txt"; // NOLINT
static const string SAVE_MISSION_PATH = "../save/saveMission.txt";
class Player;
/*
 * @brief ������
 */
class Mission {
public:
    explicit Mission(int id);

    void showDescription();
    bool checkFinished();
    void missionFinish(Player &player);
    void loadMission(ifstream& f,string owner, string path);

    void saveMission(string owner, string path);

public:
    int id;
    string nameEN;
    string nameCN;
    string description;
    bool isAccepted;
    bool isFinished;
    bool isProcess;
    int bonusMoney;
    int bonusExperiencePoint;
    string assigner;
    map<int, int> requiredItem;
};
#endif //GAME_MISSION_H
