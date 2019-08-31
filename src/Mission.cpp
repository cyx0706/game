//
// Created by Leo on 2019-08-31.
//

#include "Mission.h"

#include <fstream>
#include <map>

#include "Tool.h"
#include "Character.h"
#include "global.h"

extern Player player;

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
    this->bonusExperiencePoint = fromString<int>(data["bonusExperiencePoint"]);

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
    this->isAccepted = true;
    this->isFinished = false;

    for (const auto& one : data) {
        if (one.first == "id" ||
        one.first == "nameCN" || one.first == "nameEN" || one.first == "bonusMoney") {
            continue;
        } else {
            this->requiredItem.insert(pair(fromString<int>(one.first), fromString<int>(one.second)));
        }
    }
}

/*
 * @brief 检查是否能完成任务
 * 检查玩家背包的物品 与 任务完成需要的物品 进行对比
 * 若 未达到任务完成条件 输出"任务未完成" 不做任何处理
 * 若 已达到任务完成条件 输出"任务完成  "  从玩家背包中减去 任务所需物品的相应数量
 *                                    给玩家增加 经验 和 金钱
 */
void Mission::checkFinished() {
    // 遍历 任务所需的相应物品 比较 玩家背包中现有的数量
    for (const auto& one : this->requiredItem) {
        // 如果 玩家背包里对应物品的数量少于所需的数量 输出"任务未完成" return
        if (player.getItem(one.first) < one.second) {
            cout << "任务未完成" << endl;
            return;
        }
    }

    // 减去玩家背包中相应的物品数量
    for (const auto& one : this->requiredItem) {
        player.deleteItem(one.first, one.second);
    }

    // 任务结算
    cout << "任务完成" << endl;
    this->isFinished = true;
    // TODO 增加经验 增加金钱

}

