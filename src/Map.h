//
// Created by Leo on 2019-08-26.
//
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <map>
#include <memory>
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "GameLoop.h"
#include "global.h"

using namespace std;

extern COORD uPos;
class Map{
public:
    friend class GameLoop;
    int id;  //根据这个在文件里查找
    char edgeSign;
    COORD initPos;  // 记录用户的初始化位置

    void initMap();
    void move(int key); // 用户移动来调用这个
    void gotoxy(COORD pos);
    void print(char playerChar = 'P');  // 显示用户
    void clean(COORD clPos); // 清除
    int checkEvent(); // 检查事件
    void load(int mapId); // 载入地图
    unique_ptr<Map> nextMap(int mapId);
    void showDescription();
    void checkSpecialScene();
private:
    vector<short>edgeLeft;  // 不能在类里声明大小,除非声明为静态
    vector<short>edgeRight;  // 左右边界
    vector<short>doorPosTop;  // 上下侧的门
    vector<short>doorPosBottom;
    vector<COORD>barrier;
    map<COORD, int>items;       //映射对应物品id
    map<COORD, string>npcs;    // 映射对应的NPC
    map<COORD, string>monsters;    // 映射对应的怪

//    vector<Monsters>monsters; // 地图里的怪物
    string nameCN;
    string nameEN;
    map<COORD, int>roadTo;   //地图联通
    map<COORD, COORD>road;   //道路联通
    bool checkBottomMapTransition();
    bool checkTopMapTransition();
    void initChar(char playerChar = 'P');
    void initBarrier();
};


#endif //GAME_MAP_H
