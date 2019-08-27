//
// Created by rhc on 2019/8/26.
//

#ifndef MUD_PACKAGE_H
#define MUD_PACKAGE_H

#include <iostream>
#include <queue>
#include "Character.h"

using namespace std;

class Package {
public:
    //属性
    queue<Item> items;
    int maxItem;
    //方法
    bool addItem(int itemId);
    void showItems();
    void showItem(int itemId);
    void deleteItem(int itemId);
};
class Shop : public Package{
    void buy(Player& player,int itemId);
    void sell(Player& player,int itemId);
    void shopMenu();
};


#endif //MUD_PACKAGE_H