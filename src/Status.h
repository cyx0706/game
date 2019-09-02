//
// Created by rhc on 2019/8/26.
//

#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "global.h"

static const string Buff_TXT_PATH = "../data/buff.txt";
string SAVE_STATUS_PATH = "../data/saveStatus.txt";
string SAVE_BUFF_PATH = "../data/saveBuff.txt";

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
    void loadStatus(string owner);
};

class Buff : public Status{
public:
    Buff(string id);

    Buff();

    Buff(string name, string description, int duration);

    //����
    string name;
    string description;
    int duration;
    //����
    void showDescription();
    void saveBuff(string owner);
    void loadBuff(string owner);
};

#endif //GAME_STATUS_H
