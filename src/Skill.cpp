//
// Created by iwannaeat on 2019/8/31.
//

#include <iostream>
#include <fstream>
#include "Skill.h"
#include "Status.h"
#include "Tool.h"
#include "templateHeader.h"

using namespace std;

Skill::Skill(string id) {
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
    this->MP = fromString<int>(data["MP"]);
    this->HP = fromString<int>(data["HP"]);
    this->ATK = fromString<int>(data["ATK"]);
    if(fromString<int>(data["duration"]) != 0){
        buff = Buff(id);
    }
    f.close();
}

Skill::Skill() {
    name = "";
    ATK = 0;
    HP = 0;
    MP = 0;
    description = "";
    buff = Buff();
}

