//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_SKILL_H
#define GAME_SKILL_H

#include "global.h"
#include "Status.h"

static const string SAVE_SKILL_PATH = "../save/saveBuff.txt";

class Skill{
public:
    Skill();
    explicit Skill(string id);
    void saveSkill(string owner);
    void loadSkill(string owner);
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
