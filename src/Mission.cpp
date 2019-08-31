//
// Created by Leo on 2019-08-31.
//

#include "Mission.h"

#include <fstream>
#include <map>

#include "Tool.h"
#include "global.h"

Mission::Mission(int id) {
    ifstream f(MISSION_TXT_PATH);
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
    this->bonusMoney = fromString<int>(data["bonusMoney"]);

    f.close();

//    f.open(MISSION_TXT_PATH);
//    while (getline(f, str)) {
//        if (!str.empty()) {
//            vector<string> idLine = Tool::split(str);
//            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
//                break;
//            }
//        }
//    }

    // TODO 从存档中读出
    // this->isAccepted
    // this->isFinished

    for (const auto& one : data) {
        if (one.first == "id" ||
        one.first == "nameCN" || one.first == "nameEN" || one.first == "bonusMoney") {
            continue;
        } else {
            this->requiredItem.insert(pair(fromString<int>(one.first), fromString<int>(one.second)));
        }
    }
}

void Mission::checkFinished() {
    // TODO
    
}

