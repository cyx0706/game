//
// Created by rhc on 2019/8/26.
//
#ifndef GAME_PACKAGE_H
#define GAME_PACKAGE_H
#include <iostream>
#include <vector>
#include "Map.h"
#include "Item.h"
#include "global.h"
#include "templateHeader.h"
static const string SHOP_FILE_PATH = "../save/shop.txt";

class Shop{
public:
    Shop();
    bool buy(int itemId, int number, int &money);
    bool sell(Item &item, int number, int &money);
    void shopMenu(SCOORD &pos);
    void save();
    Package<Weapon> weaponPackage;
    Package<Armor> armorPackage;
    Package<Drug> drugPackage;
};


#endif //GAME_PACKAGE_H