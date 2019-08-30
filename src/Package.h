//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_PACKAGE_H
#define GAME_PACKAGE_H
#include "global.h"
#include <iostream>
#include <vector>
#include "Character.h"
#include "Item.h"
#include <iostream>
class Package {
public:
    //属性
    Package();
    vector<Item> items;
    int maxItem;
    //方法
    bool addItem(int itemId, int number);
    void showItems();
    void showItem(int itemId);
    bool deleteItem(int itemId, int number);
    friend istream& operator>>(istream& fpStream, Package &pack);
};

class Shop{
public:
    Shop();
    bool buy(int itemId, int number, int &money)
    bool sell(int itemId, int number, int &money)
    void shopMenu();
    Package aPackage;
};


#endif //GAME_PACKAGE_H