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

/*
 * @brief 从文件中读取 Mission 类的基本信息 构造相应的对象
 *
 * @param id Mission 对象对应的 id
 *
 * mission.txt 文件内容 (大致格式):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * nameEN
 * nameCN
 * bonusMoney
 * bonusExperiencePoint
 * __ __
 * __ __
 * __ __
 *
 * __ __ 代表 任务完成所需的物品
 * 第一个 __ 为 对应物品的 id
 * 第二个 __ 为 对应物品要求的数量
 *
 * 分别对应 Mission 类的各个属性
 * 所有键值对必须存在
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * 属性
 * isAccepted
 * isFinished
 * isProcess
 * 需要从存档中读取
 */
Mission::Mission(int id) {
    ifstream f(MISSION_TXT_PATH);
    string str;

    // 找到对应 id 处
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // 将 对应 id 行到下一个空行之间的内容读取为键值对
    map<string, string> data = Tool::dataMap(f);

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->bonusMoney = fromString<int>(data["bonusMoney"]);
    this->bonusExperiencePoint = fromString<int>(data["bonusExperiencePoint"]);

    f.close();

    // TODO 从存档中读出
    this->isAccepted = true;
    this->isFinished = false;
    this->isProcess = false;

    // 提取任务完成所需的物品 物品id 与 物品数量 的键值对，存入 requiredItem
    for (const auto& one : data) {
        // 剔除 map 中其他的键值对 id nameCN nameEN bonusMoney bonusExperiencePoint
        if (one.first == "id" ||
        one.first == "nameCN" || one.first == "nameEN" ||
        one.first == "bonusMoney" || one.first == "bonusExperiencePoint") {
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
    // 玩家 增加金钱 和 经验
    player.addMoney(this->bonusMoney);
    player.addExp(this->bonusExperiencePoint);
}

