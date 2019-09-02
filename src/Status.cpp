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
Status::Status() {
    this->HP = 0;
    this->MP = 0;
    this->DEF = 0;
    this->ATK = 0;
    this->Speed = 0;
    this->Phy = 0;
    this->Critical = 0;
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

    // �ҵ���Ӧ id ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && idLine[1] == id) {
                break;
            }
        }
    }
    // �� ��Ӧ id �е���һ������֮������ݶ�ȡΪ��ֵ��
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
