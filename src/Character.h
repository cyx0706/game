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
#include "Map.h"
#include "global.h"

static const string NPC_FILE_PATH = "../data/npcs.txt"; // ��ʼ��npc���ļ�
static const string SAVE_NPC_PATH = "../save/saveNPC.txt"; // npc��̬��ȡ���ļ�
static const string SAVE_PLAYER_PATH = "../save/savePlayerAttribute.txt";
static const string READ_MONSTER_PATH = "../data/monsters.txt";
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
    //����
    virtual bool isDead();
    virtual void showDescription();
};

class Player : public Character {
public:
//    Player(Package& package, Armor armor, Weapon weapon, Status status);
    Player();
    string talkTo;
    int money;
    int maxHP;
    int maxMP;
    void addMoney(int addition);
    void addExp(int addition);
    void levelUp();

    bool equipArmor(string &name);
    bool equipWeapon(string &name);

    void showArmors();
    void showWeapons();
    void showDrugs();
    void showItems();

    void addBuff(Buff &buff);
    void deleteBuff(Buff &buff);
    bool addMission(Mission &mission);

    void showMissions();
    void showMission();
    Mission* getMission(string& assignerId); // ��������
    Mission* getMission(int missionId);

    void showStatus(); //��ʾ�������Ժ�װ��
    void showSkills();
    void battleBagShow(SCOORD &pos);
    void playerMenu(); //��ʾ����


//    void showKilledMonster();
//    int getKilledMonster(string id);
//    void addKilledMonster();

    void addItem(int itemId, int number = 1);
    void deleteItem(int itemId, int number = 1);
    void eraseItem(int itemId);
    int getItem(int itemId);
    bool showItem(int itemId);
    bool useDrug(string& name, Character& character);

    bool isDead() override ;
    void deadScene(); // ��������

    void save();
    void load();

private:
    //����
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
//    map<string, int>killedMonster;
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
    explicit NPC(string id); // ����id��ȡ�ļ�����
    friend istream& operator>>(istream &fpStream, NPC &npc);
    void NPCMenu();
    void assignQuest(Player& player);
    void finishQuest(Player& player);
    void talk(Player &player);
    void buy(int itemId, int number, Player& player);
    void sell(Item &item, int number, Player& player);
    void setVisibility(bool isVisible);
    bool getVisibility();
    bool isDead() override ;
    void showDescription() override ;
    map<int, TalkContent>talkContent; //��ͬ����Ĳ�ͬ�Ի�
    bool missionStatus;
    bool forceBattleCheck(Player &player);
    void save();
    void load();
private:
    static Shop store;
    vector <Mission> questList;
    bool shopStatus;
    bool battleStatus;
    bool bar;
    bool isVisible;
    bool needSave;
    //����
};



#endif //GAME_CHARACTER_H
