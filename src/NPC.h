//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_NPC_H
#define GAME_NPC_H

#include "Character.h"
#include "Weapon.h"
#include "Armor.h"
#include "Player.h"

class NPC : public Character{
public:
    //属性
    Shop store;
    vector <int> questList;
    vector <int> talkList;
    bool shopStatus;
    bool battleStatus;
    Weapon weapon;
    Armor armor;
    bool bar;
    bool isVisible;
    //方法
    void assignQuest(Player& player);
    void finishQuest(Player& player,int missionId);
    void changeLocation(COORD pos,int mapId);
    void talk();
    void assignRest(Player& player);
};
#endif //GAME_NPC_H
