//
// Created by rhc on 2019/8/26.
//
#ifndef GAME_PACKAGE_H
#define GAME_PACKAGE_H
#include <iostream>
#include <vector>
#include "Item.h"
#include "global.h"
#include "templateHeader.h"

class Shop{
public:
    Shop();
    Shop(Shop &shop)= default;

    bool buy(int itemId, int number, int &money);
    bool sell(Item &item, int number, int &money);
    void shopMenu();
    void save();
    Package<Weapon> weaponPackage;
    Package<Armor> armorPackage;
    Package<Drug> drugPackage;
};


#endif //GAME_PACKAGE_H