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
#include "Mission.h"
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
    Character(Status status, string id, string nameCN,
              string nameEN, string description,
              Location location, char display,
              int fallingExp = 100, int fallingMoney = 50);
    Character();
    Status status;
    string id;
    string nameEN;
    string nameCN;
    string description;
    int fallingExp;
    int fallingMoney;
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
    void addMoney(int addition);
    void addExp(int addition);
    void levelUp();

    bool equipArmor(string &name);
    bool equipWeapon(string &name);

    void showArmors();
    void showWeapons();
    void showDrugs();
    void showItems();


    bool addMission(Mission &mission);
    void showMissions();
    void showMission(int missionId);
    Mission* getMission(string assignerId); // 返回任务

    void showStatus(); //显示人物属性和装备
    void playerMenu(); //提示命令


//    void showKilledMonster();
//    int getKilledMonster(string id);
//    void addKilledMonster();

    void addItem(int itemId, int number = 1);
    void deleteItem(int itemId, int number = 1);
    void eraseItem(int itemId);
    int getItem(int itemId);

    bool isDead() override ;
    void deadScene(); // 死亡场景

//    void save();
//    void load();

private:
    //属性
    Package<Weapon> weaponBag;
    Package<Armor> armorBag;
    Package<Drug> drugBag;
    Package<Item> itemBag;
    int experiencePoint;
    vector <Mission> quests;
    int days;
    Weapon weapon;
    Armor armor;
    int money;
    int Lv;
    map<string, int>killedMonster;
};

class Monster : public Character {
public:
    explicit Monster(string id);
    vector<Item>fallingItem;
};


class NPC : public Character{
public:
    explicit NPC(string id); // 根据id读取文件构造
    NPC(NPC& npc) = default;
    void NPCMenu();
    void assignQuest(Player& player);
    void finishQuest(Player& player);
    void talk(Player &player);
    void buy(int itemId, int number, int &money);
    void sell(Item &item, int number, int &money);
//    void assignRest(Player& player);
    void setVisibility(bool isVisible);
    bool getVisibility();
    bool isDead() override ;
    void showDescription() override ;
    map<int, string[3]>talkContent; //不同任务的不同对话
    bool forceBattleCheck(Player &player);
private:
    Shop store;
    vector <Mission> questList;
    bool shopStatus;
    bool battleStatus;
    bool missionStatus;
    bool bar;
    bool isVisible;
    //方法
};



#endif //GAME_CHARACTER_H
