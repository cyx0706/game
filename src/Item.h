//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_ITEM_H
#define GAME_ITEM_H
#include <string>
#include "global.h"
#define ITEMS_TXT_PATH "../data/items.txt"
using std::string;

class Item{
public:
    explicit Item(int id, int number = 1);
    int id;
    string nameCN;
    string nameEN;
    string description;
    int num;
    int boughtPrice;
    bool canDrop;
    bool canSell;
    void showDescription();
};

class Drug:public Item{

};

class Weapon: public Item{
public:
    Weapon()= default;
};

class Armor: public Item{
public:
    Armor()= default;
    int ATK = 0;
    int DEF = 0;
    int Speed = 0;
};
#endif //GAME_ITEM_H
