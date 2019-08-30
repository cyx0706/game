//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MONSTER_H
#define GAME_MONSTER_H

#include "Character.h"
#include "Item.h"

class Monster : public Character{
public:
    vector<Item>fallingItem;
    int fallingExp;
    int fallingMoney;
    vector<int>missionTarget;
};
#endif //GAME_MONSTER_H
