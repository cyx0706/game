//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "global.h"

class Status {
public:
    Status()= default;
    //属性
    int HP = 100;//生命值，为0时死亡
    int MP = 100;//魔法值，使用技能时消耗
    int Phy = 10;//体力值，影响可行动次数，最大值10
    int Vit = 10;//耐力，影响防御力DEF
    int Agi;//战斗时速度Speed
    int Wis;
    int Str;//力量，影响攻击力ATK
    int ATK = 10;//攻击力
    int Speed;
    int Critical;
    int DEF;//防御力
};
class Buff : public Status{
public:
    Buff()= default;
    //属性
    string name;
    string description;
    int duration;
    //方法
    void showDescription();
};

#endif //GAME_STATUS_H
