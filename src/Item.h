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
    virtual void showDescription();

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
    int HP;
    int MP;
    int Phy;
    bool playerTarget;

    void showDescription() override;
};

/*
 * @brief 武器类
 */
class Weapon: public Item{
public:
    Weapon();
    explicit Weapon(int id, int number = 1);
    int ATK;
    int DEF;
    int Speed;

    void showDescription() override;
};

/*
 * @brief 防具类
 */
class Armor: public Item{
public:
    Armor();
    explicit Armor(int id, int number = 1);
    int ATK = 0;
    int DEF = 0;
    int Speed = 0;

    void showDescription() override;
};

#endif //GAME_ITEM_H
