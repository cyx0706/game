//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <iostream>
#include "Character.h"
#include "Weapon.h"
#include "Armor.h"
#include "Skill.h"
#include "NPC.h"
#include "Item.h"

using namespace std;

class Player : public Character{
public:
    //属性
    Package bag;
    int experiencePoint = 0;
    vector <int> quests;//Mission的id
    int days = 0;
    Weapon weapon;
    Armor armor;
    int money = 0;
    //方法
    void levelUp();
    bool equipArmor(Armor& armor,int armorId);
    void showArmor();
    void showWeapon();
    bool equipWeapon(Weapon& weaon,int armorId);
    void save(NPC& npcs[]);
    void moveTo(COORD pos);
    void addMission(int missionId);
    void showMissions();
    void showMission(int missionId);
    void missionProcess();
    void showStatus();
    void playerMenu();
    void addSkill(Skill& skill);
    void addItem(Item &item);
    virtual bool isDead();
    //构造函数
    Player();
};

Player::Player() {

}

void Player::levelUp() {
    Lv += 1;
}
#endif //GAME_PLAYER_H
