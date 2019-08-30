//
// Created by rhc on 2019/8/26.
//

#ifndef MUD_PACKAGE_H
#define MUD_PACKAGE_H
#include "global.h"
#include <iostream>
#include <vector>
#include "Character.h"
#include "Item.h"

class Package {
public:
    //属性
    Package()= default;
    vector<Item> items;
    int maxItem;
    //方法
    bool addItem(int itemId);
    void showItems();
    void showItem(int itemId);
    void deleteItem(int itemId);
};

class Shop : public Package{
public:
    Shop()= default;
    void buy(int itemId);
    void sell(int itemId);
    void shopMenu();
};


#endif //MUD_PACKAGE_H