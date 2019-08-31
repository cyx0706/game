#include "Status.h"

//
// Created by cyx on 2019/8/31.
//
Status::Status(int HP, int MP, int ATK, int PHY, int DEF, int CRITICAL, int SPEED) {
    this->HP = HP;
    this->DEF = DEF;
    this->ATK = ATK;
    this->Critical = CRITICAL;
    this->Speed = SPEED;
    this->MP = MP;
    this->Phy = PHY;
}
