//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_SKILL_H
#define GAME_SKILL_H

#include "global.h"
class Skill{
public:
    Skill();
    Skill(string id);
    vector <Buff> buffs;
    string description;
    int HP;
    int MP;
};
#endif //GAME_SKILL_H
