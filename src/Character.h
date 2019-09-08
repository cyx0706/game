//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <vector>
#include <map>
#include "Package.h"
#include "Status.h"
#include "Item.h"
#include "Skill.h"
#include "Mission.h"
#include "global.h"

static const string NPC_FILE_PATH = "../data/npcs.txt"; // 初始化npc的文件
static const string SAVE_NPC_PATH = "../save/saveNPC.txt"; // npc动态读取的文件
static const string SAVE_PLAYER_PATH = "../save/savePlayerAttribute.txt";
static const string READ_MONSTER_PATH = "../data/monsters.txt";
static const string INIT_NPC_PATH = "../data/initNPC.txt"; // 新游戏的npc的读取
class Mission;
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
    int money;
    int maxHP;
    int maxMP;
    void addMoney(int addition);
    void addExp(int addition);
    void levelUp();

    bool equipArmor(string &name);
    bool equipWeapon(string &name);
    bool takeOffArmor(string &name);
    bool takeOffWeapon(string &name);
    Armor* getArmor();
    Weapon* getWeapon();

    void showArmors();
    void showWeapons();
    void showDrugs();
    void showItems();

    void addBuff(Buff &buff);
    void deleteBuff(Buff &buff);
    bool addMission(Mission &mission);

    void showMissions();
    void showMission();
    Mission* getMission(string &assignerName); // 返回任务
    Mission* getMission(int missionId);

    void showStatus(); //显示人物属性和装备
    void showSkills();
    void battleBagShow(SCOORD &pos);
    void playerMenu(); //提示命令

    void addItem(int itemId, int number = 1);
    void deleteItem(int itemId, int number = 1);
    void eraseItem(int itemId);
    int getItem(int itemId);
    bool showItem(int itemId);
    bool useDrug(string& name, Character& character);

    bool isDead() override ;
    void deadScene(); // 死亡场景

    void save();
    void load();

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
    int Lv;
};


class Monster : public Character {
public:
    explicit Monster(string id);
    vector<Item>fallingItem;
};

struct TalkContent{
    string start;
    string process;
    string end;
};


class NPC : public Character{
public:

    map<int, TalkContent>talkContent; //不同任务的不同对话
    bool missionStatus;
    static int readLastLine;
    explicit NPC(string id, string path = SAVE_NPC_PATH); // 根据id读取文件构造
    friend istream& operator>>(istream &fpStream, NPC &npc);
    bool NPCMenu(Player &player);
    void shopMenu(Player &player);
    void assignQuest(Player& player);
    void finishQuest(Player& player);
    void talk(Player &player);
    bool buy(int itemId, int number, Player& player);
    bool sell(Item &item, int number, Player& player);

    void setVisibility(bool isVisible);
    bool getVisibility();
    bool getShopStatus();
    bool isDead() override ;
    void showDescription() override ;

    static void storeSave();
    static void save();
    void load(string path);
private:
    static Shop store;
    bool specialEvent(Player &player);

    vector <Mission> questList;
    bool shopStatus;
    bool battleStatus;
    bool bar;
    bool isVisible;
    bool needSave;
};



#endif //GAME_CHARACTER_H
