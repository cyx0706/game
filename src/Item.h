//
// Created by Leo on 2019-08-26.
//

#ifndef GAME_ITEM_H
#define GAME_ITEM_H

#include <string>

class Item{
public:
    int id;
    std::string nameCN;
    std::string nameEN;
    std::string description;
    int num;
    int boughtPrice;
    bool canDrop;
    bool canSell;
};
#endif //GAME_ITEM_H
