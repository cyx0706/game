//
// Created by cyx on 2019/8/31.
//
#include <fstream>
#include "Status.h"
#include "Tool.h"
#include "templateHeader.h"
#include "global.h"

Status::Status(int HP, int MP, int ATK, int PHY, int DEF, int CRITICAL, int SPEED) {
    this->HP = HP;
    this->DEF = DEF;
    this->ATK = ATK;
    this->Critical = CRITICAL;
    this->Speed = SPEED;
    this->MP = MP;
    this->Phy = PHY;
}
Status::Status() {
    this->HP = 0;
    this->MP = 0;
    this->DEF = 0;
    this->ATK = 0;
    this->Speed = 0;
    this->Phy = 0;
    this->Critical = 0;
}

void Status::saveStatus(string owner) {
    ofstream of;
    of.open(SAVE_STATUS_PATH);
    map<string,string> m_map;
    //保存player的单项属性
    m_map["owner"] = owner;
    m_map["HP"] = toString<int>(HP);
    m_map["MP"] = toString<int>(MP);
    m_map["Phy"] = toString<int>(Phy);
    m_map["ATK"] = toString<int>(ATK);
    m_map["Speed"] = toString<int>(Speed);
    m_map["Critical"] = toString<int>(Critical);
    m_map["DEF"] = toString<int>(DEF);
    auto iter = m_map.begin();
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    m_map.clear();
    of << endl;
    of.close();
}

void Status::loadStatus(string owner, string path) {
    ifstream f(path);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "owner" && idLine[1] == owner) {
                break;
            }
        }
    }

    map<string, string> data = Tool::dataMap(f);

    this->MP = fromString<int>(data["MP"]);
    this->HP = fromString<int>(data["HP"]);
    this->Phy = fromString<int>(data["Phy"]);
    this->ATK = fromString<int>(data["ATK"]);
    this->Speed = fromString<int>(data["Speed"]);
    this->Critical = fromString<int>(data["Critical"]);
    this->DEF = fromString<int>(data["DEF"]);
    f.close();
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
void Buff::showDescription() {

}

