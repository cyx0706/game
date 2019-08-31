//
// Created by cyx on 2019/8/30.
//
#include "Character.h"
#include "Item.h"
#include "Status.h"

bool Character::isDead() {
    return (this->status.HP < 0);
}

void Character::showDescription() {
    cout << this->description << endl;
}

Character::Character(Status status) :status(status){

}

Character::Character() {
    Status status1;
    this->status = status1;
}



NPC::NPC(Status status,
         vector<int> quests,
         bool isVisible,
         bool battleStatus,
         bool shopStatus,
         bool bar,
         Weapon weapon,
         Armor armor,
         Shop shop)
         : Character(status), store(shop), weapon(weapon), armor(armor) {
    this->isVisible = isVisible;
    this->battleStatus = battleStatus;
    this->bar = bar;
    this->shopStatus = shopStatus;
    this->questList = quests;
}


Player::Player(Package &package,
               Armor armor,
               Weapon weapon,
               Status status)
               : bag(package), armor(armor), weapon(weapon), Character(status){

}

Player::Player(): Character() {
    this->weapon
}
