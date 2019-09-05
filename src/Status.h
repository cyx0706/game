//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "global.h"

static const string BUFF_TXT_PATH = "../data/skill.txt";
static const string SAVE_STATUS_PATH = "../save/saveStatus.txt";
static const string READ_MONSTER_STATUS_PATH = "../data/characterStatus.txt";

class Status {
public:
    explicit Status(int HP, int MP, int ATK = 15, int PHY = 4,
                    int DEF = 2, int CRITICAL = 10, int SPEED = 5);
    //����
    Status();
    int HP; //����ֵ��Ϊ0ʱ����
    int MP; //ħ��ֵ��ʹ�ü���ʱ����
    int Phy; //����ֵ��Ӱ����ж����������ֵ10
    int ATK; //������
    int Speed;
    int Critical;
    int DEF; //������
    //����
    void saveStatus(string owner);
    void loadStatus(string owner, string path);
};

class Buff : public Status{
public:

    Buff();
    Buff(string name, string description, int duration);

    //����
    string name;
    string description;
    int duration;
    //����
    void showDescription();
};

#endif //GAME_STATUS_H
