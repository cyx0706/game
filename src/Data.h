//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <iostream>
#include <vector>
#include "Status.h"

using namespace std;

class Drug{
    vector <Buff> buff;
    int HP;
    int MP;
    int Phy;
};

class Skill {
public:
    vector<Buff> buffs;
    string description;
    int HP;
    int MP;
};

class Weapon{
public:
    int ATK;
    int DEF;
    int Speed;
    Skill skill;
};

#endif //GAME_DATA_H
