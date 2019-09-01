//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "global.h"

static const string Buff_TXT_PATH = "../data/buff.txt";

class Status {
public:
    explicit Status(int HP, int MP, int ATK = 15, int PHY = 4,
                    int DEF = 2, int CRITICAL = 10, int SPEED = 5);
    //属性
    Status();
    int HP; //生命值，为0时死亡
    int MP; //魔法值，使用技能时消耗
    int Phy; //体力值，影响可行动次数，最大值10
    int ATK; //攻击力
    int Speed;
    int Critical;
    int DEF; //防御力
};

class Buff : public Status{
public:
    Buff(string id);

    Buff();

    Buff(string name, string description, int duration);

    //属性
    string name;
    string description;
    int duration;
    //方法
    void showDescription();
};

#endif //GAME_STATUS_H
