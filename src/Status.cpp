//
// Created by cyx on 2019/8/31.
//
#include <fstream>
#include "Status.h"
#include "Tool.h"
#include "templateHeader.h"

Status::Status(int HP, int MP, int ATK, int PHY, int DEF, int CRITICAL, int SPEED) {
    this->HP = HP;
    this->DEF = DEF;
    this->ATK = ATK;
    this->Critical = CRITICAL;
    this->Speed = SPEED;
    this->MP = MP;
    this->Phy = PHY;
}

Buff::Buff():Status() {
    description = "";
    name = "";
    duration = 0;
}

Buff::Buff(string name, string description, int duration):Status() {
    this->duration = duration;
    this->name = name;
    this->description = description;
}

Buff::Buff(string id):Status() {
    ifstream f(Buff_TXT_PATH);
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

    this->name = data["name"];
    this->description = data["description"];
    this->duration = fromString<int>(data["duration"]);
    this->MP = fromString<int>(data["MP"]);
    this->HP = fromString<int>(data["HP"]);
    this->Phy = fromString<int>(data["Phy"]);
    this->ATK = fromString<int>(data["ATK"]);
    this->Speed = fromString<int>(data["Speed"]);
    this->Critical = fromString<int>(data["Critical"]);
    this->DEF = fromString<int>(data["DEF"]);
    f.close();
}

void Buff::showDescription() {

}
