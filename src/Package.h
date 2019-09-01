//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_PACKAGE_H
#define GAME_PACKAGE_H
#include <iostream>
#include "global.h"
#include <iostream>
#include <vector>
#include "Item.h"

template <class T>
class Package {
public:
    //属性
    explicit Package(int max = 25);
    vector<T> items;
    int maxItem; // 每个背包的最大大小
    //方法
    bool addItem(int itemId, int number);
    void showItems();
    void showItem(int itemId);
    bool deleteItem(int itemId, int number);
    bool deleteItem(int itemId);
    friend istream& operator>>(istream& fpStream, Package<T> &pack);
};

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