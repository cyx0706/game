//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_ITEM_H
#define GAME_ITEM_H

#include <string>
#include "global.h"

static const string ITEMS_TXT_PATH = "../data/items.txt"; // NOLINT

class Item{
public:
    int id;
    string nameCN;
    string nameEN;
    string description;
    int num;
    int boughtPrice;
    bool canDrop;
    bool canSell;
public:
    Item(int id);

};

#endif //GAME_ITEM_H
