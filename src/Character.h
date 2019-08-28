//
// Created by rhc on 2019/8/26.
//

#ifndef MUD_CHARACTER_H
#define MUD_CHARACTER_H

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

using namespace std;

class Character {
public:
    //属性
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
#endif //MUD_CHARACTER_H
