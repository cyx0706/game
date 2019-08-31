//
// Created by Leo on 2019-08-30.
//

#include "Item.h"

#include <map>
#include <fstream>

#include "Tool.h"
#include "global.h"

/*
 * @brief 从文件中读取出 item 类的基本信息
 * @param id item 对象对应的表示 id
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

void Item::showDescription() {
    if (id != 0) {
        cout << this->description << endl;
    } else {
        cout << "无" << endl;
    }
}


Weapon::Weapon() :Item(){

}

Armor::Armor() :Item(){

}
