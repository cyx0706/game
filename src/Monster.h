//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_MONSTER_H
#define GAME_MONSTER_H

#include "Character.h"
#include "Item.h"

class Monster : public Character{
public:
    Item fallingItem[10];
    int fallingExp;
    int fallingMoney;
    int missionTarget[10];
};
#endif //GAME_MONSTER_H
