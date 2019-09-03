//
// Created by iwannaeat on 2019/8/31.
//

#include <iostream>
#include <fstream>
#include "Skill.h"
#include "Status.h"
#include "Tool.h"
#include "templateHeader.h"
#include "global.h"


Skill::Skill(string id) {
    ifstream f(BUFF_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && idLine[1] == id) {
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
    this->MP = fromString<int>(data["MP"]);
    this->HP = fromString<int>(data["HP"]);
    this->ATK = fromString<int>(data["ATK"]);
    //如果duration不为0，此skill为加buff，请使用buff中的属性值
    this->buff = Buff();
    this->buff.name = this->nameEN;
    this->buff.ATK = fromString<int>(data["buffATK"]);
    this->buff.DEF = fromString<int>(data["buffDEF"]);
    f.close();
}

Skill::Skill() {
    nameEN = "";
    nameCN = "";
    ATK = 0;
    HP = 0;
    MP = 0;
    description = "";
    buff = Buff();
}

void Skill::saveSkill(string owner) {

}

void Skill::loadSkill(string owner) {

}

