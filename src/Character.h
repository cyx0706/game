//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <iostream>
#include <vector>
#include <wincon.h>
#include "Package.h"
#include "Status.h"
#include "Weapon.h"
#include "Armor.h"

struct Location{
    int mapId;
    int x;
    int y;
};


class Character {
public:
    //属性
    string id;
    string nameEN;
    string nameCN;
    int fallingExp;
    string description;
    Status status;
    vector <Buff> buffs;
    Location mapLocation;
    char displayChar;
    vector <Skill&> skills;
    //方法
    virtual bool isDead() = 0;
    virtual void showDescription() = 0;
};
#endif //GAME_CHARACTER_H
