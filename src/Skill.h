//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_SKILL_H
#define GAME_SKILL_H

#include "global.h"
#include "Status.h"

class Skill{
public:
    Skill();
    Skill(string id);
    Buff buff;
    string description;
    int HP;
    int MP;
    int ATK;
    string id;
    string nameCN;
    string nameEN;
};
#endif //GAME_SKILL_H
