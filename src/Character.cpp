#include "Character.h"
#include <utility>
//
// Created by cyx on 2019/8/30.
//
bool Character::isDead() {
    return (this->status.HP < 0);
}

void Character::showDescription() {
    cout << this->description << endl;
}

Character::Character(Status status) :status(status){

}



NPC::NPC(Status status, vector<int> quests, bool isVisible, bool battleStatus, bool shopStatus, bool bar,
         Weapon weapon, Armor armor, Shop shop) : Character(status), store(shop) {
}


Player::Player(Package &package,
               Armor armor,
               Weapon weapon,
               Status status)
               : bag(package), armor(std::move(armor)), weapon(std::move(weapon)), Character(status){

}
