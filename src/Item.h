//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_ITEM_H
#define GAME_ITEM_H

#include <string>
#include <vector>

#include "Status.h"
#include "Skill.h"
#include "global.h"

static const string ITEMS_TXT_PATH = "../data/items.txt"; // NOLINT
static const string DRUG_TXT_PATH = "../data/drug.txt"; // NOLINT
static const string WEAPON_TXT_PATH = "../data/weapon.txt"; //NOLINT
static const string ARMOR_TXT_PATH = "../data/armor.txt"; // NOLINT

/*
 * @brief 物品类
 */
class Item{
public:
    Item();
    explicit Item(int id, int number = 1);
    Item(Item& item) = default;

    virtual void showDescription();

public:
    int id;
    string nameCN;
    string nameEN;
    string description;
    int num;
    int boughtPrice;
    bool canDrop;
    bool canSell;
};

/*
 * @brief 药剂类
 */
class Drug: public Item{
public:
    Drug();
    explicit Drug(int id, int number = 1);
    Drug(Drug& drug) = default;

public:
    vector<Buff> buff;
    int HP;
    int MP;
    int Phy;
    bool playerTarget;
};

/*
 * @brief 武器类
 */
class Weapon: public Item{
public:
    Weapon();
    explicit Weapon(int id, int number = 1);
    Weapon(Weapon& weapon) = default;

public:
    int ATK;
    int DEF;
    int Speed;
};

/*
 * @brief 防具类
 */
class Armor: public Item{
public:
    Armor();
    explicit Armor(int id, int number = 1);
    Armor(Armor& armor) = default;
public:
    int ATK = 0;
    int DEF = 0;
    int Speed = 0;
};

#endif //GAME_ITEM_H
