//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <iostream>
#include <vector>
#include <map>
#include "Package.h"
#include "Status.h"
#include "Item.h"
#include "Skill.h"
#include "global.h"
//
//class Player;
//class NPC;
//class Monster;
struct Location{
    int mapId;
    int x;
    int y;
};


class Character {
public:
    Character(Status status);
    Character();
    Status status;
    string id;
    string nameEN;
    string nameCN;
    int fallingExp;
    string description;
    vector<Buff> buffs;
    Location mapLocation;
    char displayChar;
    vector <Skill> skills;
    //方法
    virtual bool isDead();
    virtual void showDescription();
};

class Player : public Character {
public:
//    Player(Package& package, Armor armor, Weapon weapon, Status status);
    Player();
    string talkTo;
    void addMoney();
    void addExp();
    void levelUp();

    bool equipArmor(string name);
    bool equipWeapon(string weapon);

    void showArmors();
    void showWeapons();
    void showDrugs();
    void showItems();

    void addMission(int missionId);
    void showMissions();
    void showMission(int missionId);

    void showStatus(); //显示人物属性和装备
    void playerMenu(); //提示命令

    void addSkill(string skillId);

    void showKilledMonster();
    int getKilledMonster(string id);
    void addKilledMonster();

    void addItem(int itemId, int number = 1);
    void deleteItem(int itemId, int number = 1);
    void eraseItem(int itemId);
    int getItem(int itemId);

    virtual bool isDead();
    void deadScene(); // 死亡场景

    void save();
    void load();

private:
    //属性
    Package<Weapon> weaponBag;
    Package<Armor> armorBag;
    Package<Drug> drugBag;
    Package<Item> itemBag;
    int experiencePoint = 0;
    vector <int> quests;
    int days = 0;
    Weapon weapon;
    Armor armor;
    int money = 0;
    int Lv = 1;
    map<string, int>killedMonster;
};

class Monster : public Character {
public:
    Monster()= default;
    vector<Item>fallingItem;
    int fallingExp;
    int fallingMoney;
    virtual bool isDead();
};



class NPC : public Character{
public:
    NPC(Status status, vector<int> quests, bool isVisible, bool battleStatus, bool shopStatus, bool bar,
        Weapon weapon, Armor armor, Shop shop);
    void shop();
    void assignQuest(Player& player);
    void finishQuest(Player& player,int missionId);
//    void changeLocation(COORD pos,int mapId);
    void talk();
    void assignRest(Player& player);
    void setVisibility(bool isVisible);
    bool getVisibility();
    virtual bool isDead();
//    static map<string,  >
private:
    Shop store;
    vector <int> questList;
    bool shopStatus;
    bool battleStatus;
    Weapon weapon;
    Armor armor;
    bool bar;
    bool isVisible;
    //方法

};



#endif //GAME_CHARACTER_H
