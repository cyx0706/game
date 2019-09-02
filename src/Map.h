//
// Created by Leo on 2019-08-26.
//
#ifndef GAME_MAP_H
#define GAME_MAP_H
extern "C"{
#include <windows.h>
#include <conio.h>
}
#include <algorithm>
#include <vector>
#include <map>

#include "Item.h"
#include "Character.h"
#include "global.h"

struct SCOORD:public COORD{
    bool operator<(const SCOORD &pos);
    bool operator>(const SCOORD &pos);
    bool operator<(const SCOORD &pos) const ;
    bool operator>(const SCOORD &pos) const ;
};


class Map{
public:
    friend class GameLoop;
    int id;  //����������ļ������
    char edgeSign;
    COORD initPos;  // ��¼�û��ĳ�ʼ��λ��

    void initMap();
    void move(int key); // �û��ƶ����������
    void gotoxy(SCOORD pos);
    void print(char playerChar = 'P');  // ��ʾ�û�
    void clean(SCOORD clPos); // ���
    int checkEvent(); // ����¼�
    void load(int mapId); // �����ͼ
    void nextMap(int mapId);
    void showDescription();
    void checkSpecialScene();
private:
    vector<short>edgeLeft;  // ����������������С,��������Ϊ��̬
    vector<short>edgeRight;  // ���ұ߽�
    vector<short>doorPosTop;  // ���²����
    vector<short>doorPosBottom;
    vector<SCOORD>barrier;
    map<SCOORD, int>items;       //ӳ���Ӧ��Ʒid
    map<SCOORD, string>npcs;    // ӳ���Ӧ��NPC
    map<SCOORD, string>monsters;    // ӳ���Ӧ�Ĺ�
    string nameCN;
    string nameEN;
    map<SCOORD, int>roadTo;   //��ͼ��ͨ
    map<SCOORD, SCOORD>road;   //��·��ͨ
    bool checkBottomMapTransition();
    bool checkTopMapTransition();
    void initChar(char playerChar = 'P');
    void initBarrier();
};


#endif //GAME_MAP_H
