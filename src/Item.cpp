//
// Created by Leo on 2019-08-30.
//

#include "Item.h"

#include <map>
#include <fstream>

#include "Tool.h"
#include "global.h"

/*
 * @brief 无参的构造函数 用于物品类的站位 即为空
 */
Item::Item() {
    this->id = 0;
    this->nameCN = "";
    this->nameEN = "";
    this->description = "";
    this->num = 0;
    this->boughtPrice = 0;
    this->canDrop = false;
    this->canSell = false;
}

/*
 * @brief 从文件中读取出 item 类的基本信息
 *
 * @param id item 对象对应的表示 id
 * @param number 对象物品的数量
 */
Item::Item(int id, int number) {
    ifstream f(ITEMS_TXT_PATH);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(f);

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->description = data["description"];
    this->num = number;
    this->boughtPrice = fromString<int>(data["boughtPrice"]);
    this->canDrop = Tool::boolFromString(data["canDrop"]);
    this->canSell = Tool::boolFromString(data["canSell"]);

    f.close();
}
/*
 * @brief 打印物品的介绍
 */
void Item::showDescription() {
    if (id != 0) {
        cout << this->description << endl;
    } else {
        cout << "无" << endl;
    }
}

/*
 * @brief 无参的构造函数 即为空
 */
Drug::Drug() : Item(){
    this->HP = 0;
    this->MP = 0;
    this->Phy = 0;
    this->playerTarget = false;
}

Drug::Drug(int id, int number) : Item(id, number){
    ifstream f(DRUG_TXT_PATH);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(f);

    this->HP = fromString<int>(data["HP"]);
    this->MP = fromString<int>(data["MP"]);
    this->Phy = fromString<int>(data["Phy"]);
    this->playerTarget = Tool::boolFromString(data["playerTarget"]);

    f.close();
}


Weapon::Weapon() : Item(){
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
    this->skill = Skill();
}

Weapon::Weapon(int id, int number) : Item(id, number) {
    ifstream f(WEAPON_TXT_PATH);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(f);

    this->ATK = fromString<int>(data["ATK"]);
    this->DEF = fromString<int>(data["DEF"]);
    this->Speed = fromString<int>(data["Speed"]);
    this->skill = Skill(fromString<int>(data["skill"]));

    f.close();
}


Armor::Armor() : Item(){
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
}

Armor::Armor(int id, int number) : Item(id, number) {
    ifstream f(ARMOR_TXT_PATH);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(f);

    this->ATK = fromString<int>(data["ATK"]);
    this->DEF = fromString<int>(data["DEF"]);
    this->Speed = fromString<int>(data["Speed"]);

    f.close();
}
