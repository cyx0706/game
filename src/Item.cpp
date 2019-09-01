//
// Created by Leo on 2019-08-30.
//

#include "Item.h"

#include <map>
#include <fstream>

#include "Status.h"
#include "Tool.h"
#include "global.h"

/*
 * @brief 无参的构造函数 用于物品栏的占位 即为空
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
 * @brief 从文件中读取出 Item 类的基本信息 构造相应的对象
 *
 * @param id Item 对象对应的表示 id
 * @param number 对象物品的数量
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * 文件中物品的 id 对应:
 * 001 -- 099: 武器 Weapon
 * 101 -- 199: 防具 Armor
 * 201 -- 299: 药剂 Drug
 * 301 -- 399: 任务道具
 *
 * items.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id 1
 * nameCN 剑
 * nameEN sword
 * description 一把普通的剑，好像是量产的类型
 * boughtPrice 10
 * canDrop true
 * canSell true
 *
 * 分别对应 Item 类的各个属性
 * 所有键值对必须存在
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Item::Item(int id, int number) {
    ifstream f(ITEMS_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && Tool::fromStringTo<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // 将 对应 id 行到下一个空行之间的内容读取为键值对
    map<string, string> data = Tool::dataMap(f);

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->description = data["description"];
    this->num = number;
    this->boughtPrice = Tool::fromStringTo<int>(data["boughtPrice"]);
    this->canDrop = Tool::boolFromString(data["canDrop"]);
    this->canSell = Tool::boolFromString(data["canSell"]);

    f.close();
}

/*
 * @brief 打印物品的介绍
 */
void Item::showDescription() {
    // 剔除 占位 Item 对象，输出为无
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

/*
 * @brief 从文件中读取出 Drug 类的基本信息 构造相应的对象
 *
 * @param id Item 超类对应的 id (id 范围: 201 -- 299)
 * @param number 对象物品的数量
 *
 * drug.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * buff
 * HP
 * MP
 * Phy
 * playerTarget
 *
 * 分别对应 Drug 类的各个属性
 * 所有键值对必须存在
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Drug::Drug(int id, int number) : Item(id, number){
    ifstream f(DRUG_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && Tool::fromStringTo<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // 将 对应 id 行到下一个空行之间的内容读取为键值对
    map<string, string> data = Tool::dataMap(f);

    this->HP = Tool::fromStringTo<int>(data["HP"]);
    this->MP = Tool::fromStringTo<int>(data["MP"]);
    this->Phy = Tool::fromStringTo<int>(data["Phy"]);
    this->playerTarget = Tool::boolFromString(data["playerTarget"]);

    f.close();

    // buff 属性不是普通键值对，需要重新读取
    f.open(DRUG_TXT_PATH);

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && Tool::fromStringTo<int>(idLine[1]) == id) {
                break;
            }
        }
    }

    // 找到 id 对应的 buff 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> line = Tool::split(str);
            if (line[0] == "buff") {
                // 删除 line (vector<string>) 的第一个元素 即 字符串"buff"
                line.erase(line.begin());

                // 构造 Buff 对象，存入 buff (vector<Buff>) 中
                for (const auto& one: line) {
                    this->buff.emplace_back(one);
                }
            }
        }
    }

    f.close();
}

/*
 * @brief 无参的构造函数 即为空
 */
Weapon::Weapon() : Item(){
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
}

/*
 * @brief 从文件中读取出 Weapon 类的基本信息 构造相应的对象
 *
 * @param id Item 超类对应的 id (id 范围: 001 -- 099)
 * @param number 对象物品的数量
 *
 * weapon.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * ATK
 * DEF
 * Speed
 *
 * 分别对应 Weapon 类的各个属性
 * 所有键值对必须存在
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Weapon::Weapon(int id, int number) : Item(id, number) {
    ifstream f(WEAPON_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && Tool::fromStringTo<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // 将 对应 id 行到下一个空行之间的内容读取为键值对
    map<string, string> data = Tool::dataMap(f);

    this->ATK = Tool::fromStringTo<int>(data["ATK"]);
    this->DEF = Tool::fromStringTo<int>(data["DEF"]);
    this->Speed = Tool::fromStringTo<int>(data["Speed"]);

    f.close();
}

/*
 * @brief 无参的构造函数 即为空
 */
Armor::Armor() : Item(){
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
}

/*
 * @brief 从文件中读取出 Armor 类的基本信息 构造相应的对象
 *
 * @param id Item 超类对应的 id (id 范围: 101 -- 199)
 * @param number 对象物品的数量
 *
 * armor.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * ATK
 * DEF
 * Speed
 *
 * 分别对应 Armor 类的各个属性
 * 所有键值对必须存在
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Armor::Armor(int id, int number) : Item(id, number) {
    ifstream f(ARMOR_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && Tool::fromStringTo<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // 将 对应 id 行到下一个空行之间的内容读取为键值对
    map<string, string> data = Tool::dataMap(f);

    this->ATK = Tool::fromStringTo<int>(data["ATK"]);
    this->DEF = Tool::fromStringTo<int>(data["DEF"]);
    this->Speed = Tool::fromStringTo<int>(data["Speed"]);

    f.close();
}
