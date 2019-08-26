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
#include "Character.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
using namespace std;

#define ARRAYSIZE 20

class Map{
public:
    static HANDLE hOut;
    static CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    static CONSOLE_CURSOR_INFO cursorInfo;
    int id;  //根据这个在文件里查找
    char edgeSign;
    COORD initPos;  // 记录用户的初始化位置
    void initMap();
    void move(COORD* uPos, int key); // 用户移动来调用这个
    void gotoxy(COORD pos);
    void print(COORD &uPos, char playerChar = 'P');  // 显示用户
    void clean(COORD clPos); // 清除
    void checkEvent(COORD &uPos, Player& player, Monster &monster, NPC &npc); // 检查事件
    void load(int mapId); // 载入地图
    unique_ptr<Map> nextMap(int mapId);
private:
    vector<short>edgeLeft;  // 不能在类里声明大小,除非声明为静态
    vector<short>edgeRight;  // 左右边界
    vector<short>doorPosTop;  // 上下侧的门
    vector<short>doorPosBottom;
    vector<COORD>barrier;
//    COORD startPos;
    //  map<COORD, Item*>  //映射对应物品
//  map<COORD, NPC*>   // 映射对应的NPC
//  map<COORD, Monster*> // 映射对应的怪
//    COORD charPos;  // 记录用户的位置

    string nameCN;
    string nameEN;
    map<COORD, int>roadTo;   //地图联通
    map<COORD, COORD>road;   //道路联通
    bool checkBottomMapTransition(COORD& uPos);
    bool checkTopMapTransition(COORD& uPos);
    void initChar(char playerChar = 'P');
    void initBarrier();
};

void Map::initMap() {
    initChar();
    short temp = 0;
    bool flag = false;
    COORD pos = {0, 0};
    gotoxy(pos);
    for (int j = edgeLeft[0]; j <= edgeRight[0]; j++) {
        // 画上边界的门
        for (int z = 0; z < doorPosTop.size(); z++) {
            if (doorPosTop[z] == j){
                flag = true;
                putchar(' ');
                break;
            }
        }
        if(!flag){
            putchar(edgeSign);
        }
        flag = false;
    }
    putchar('\n');

    for (short i = 1; i < edgeLeft.size() - 1; i++) {

        if (edgeLeft[i] < edgeLeft[i+1]) {
            temp = i+1;  // 直接写i+1会出错，需要一个中间变量
            pos = {edgeLeft[i], temp};
            gotoxy(pos);
            for (short j = edgeLeft[i]; j <= edgeLeft[i+1]; j++) {
                putchar(edgeSign);
            }
        }

        if (edgeLeft[i] > edgeLeft[i+1]) {
            temp = i+1;
            pos = {edgeLeft[i+1], temp};
            gotoxy(pos);
            for (short j = edgeLeft[i+1]; j <= edgeLeft[i]; j++) {
                putchar(edgeSign);
            }
        }

        if (edgeRight[i] > edgeRight[i+1]){
            temp = i + 1;
            pos = {edgeRight[i+1], temp};
            gotoxy(pos);
            for (short j = edgeRight[i+1]; j <= edgeRight[i]; j++) {
                putchar(edgeSign);
            }
        }
        if (edgeRight[i] < edgeRight[i+1]){
            pos = {edgeRight[i], i};
            gotoxy(pos);
            for (short j = edgeRight[i]; j <= edgeRight[i+1]; j++) {
                putchar(edgeSign);
            }
        }
        pos = {edgeLeft[i], i};
        gotoxy(pos);
        putchar(edgeSign);
        pos = {edgeRight[i], i};
        gotoxy(pos);
        putchar(edgeSign);
        putchar('\n');
    }
    temp = edgeLeft.size() -1; // 无法直接使用表达式,创建临时变量
    pos = {edgeLeft[temp], temp};
    gotoxy(pos);

    flag = false;
    for (int j = edgeLeft[temp]; j <= edgeRight[temp]; j++) {
        // 画下边界的门
        for (int z = 0; z < doorPosBottom.size(); z++) {
            if (doorPosBottom[z] == j){
                flag = true;
                putchar(' ');
                break;
            }
        }
        if(!flag){
            putchar(edgeSign);
        }
        flag = false;
    }
    putchar('\n');
    initBarrier();
}

void Map::initBarrier() {
    //查一下映射的对应对象,不同的符号表示
    SetConsoleTextAttribute(hOut, 0x0a);
    GetConsoleScreenBufferInfo(hOut, &Map::screenInfo);
    CHAR_INFO chFill = {'T',  Map::screenInfo.wAttributes}; //定义剪切区域填充字符
    short xLeft = 0;
    short xRight = 0;
    for (int i = 0; i < barrier.size(); i++) {
        xLeft = barrier[i].X - 1;
        xRight = barrier[i].X;
        SMALL_RECT CutScr = {xLeft, barrier[i].Y, xRight, barrier[i].Y};
        ScrollConsoleScreenBuffer(hOut, &CutScr, nullptr, barrier[i], &chFill); //移动文本
    }
    SetConsoleTextAttribute(hOut, 0x0f);
}

//初始化用户的位置
void Map::initChar(char playerChar) {
    SetConsoleCursorPosition(hOut, initPos);
    putchar(playerChar);
}

unique_ptr<Map> Map::nextMap(int mapId) {
    auto mapNext = make_unique<Map>();
    mapNext->load(mapId); // 读取地图
    return mapNext;
}

bool Map::checkBottomMapTransition(COORD &uPos) {
    for (int i = 0; i < doorPosTop.size(); i++) {
        if (uPos.Y == doorPosTop[i]){
            return true;
        }
    }
}

bool Map::checkTopMapTransition(COORD &uPos) {
    for (int i = 0; i < doorPosBottom.size(); i++) {
        if (uPos.X == doorPosBottom[i]){
            return true;
        }
    }
}

//根据id来在文件里读
void Map::load(int mapId) {

}

//移动的函数
void Map::move(COORD *uPos, int key) {
    COORD resetPos = *uPos;
    short x = uPos->X;
    short y = uPos->Y;
    //向上走
    if(key == 72){
        bool flag1 = (y <= 1) ;
        bool flag2 = (x >= min(edgeLeft[y-1], edgeLeft[y]) && x <= max(edgeLeft[y-1], edgeLeft[y]));
        bool flag3 = (x >= min(edgeRight[y-1], edgeRight[y]) && x <= max(edgeRight[y-1], edgeRight[y]));
        if (flag1){
            checkTopMapTansition(uPos);
            return;
        }
        if (flag2||flag3){
            return;
        }
        uPos->Y--;
    }
        //向左走
    else if (key == 75){
        if (x > edgeLeft[y] + 1){
            uPos->X--;
        }
        else{
            return;
        }

    }
        //向右走
    else if (key == 77){
        if (x < edgeRight[y] - 1)
            uPos->X++;
        else{
            return;
        }
    }
        //向下走
    else if (key == 80){
        // 20大小的地图,最多走到18
        bool flag1 = (y >= edgeLeft.size() - 2) ;
        bool flag2 = (x >= min(edgeLeft[y+1], edgeLeft[y]) && x <= max(edgeLeft[y+1], edgeLeft[y]));
        bool flag3 = (x >= min(edgeRight[y+1], edgeRight[y]) && x <= max(edgeRight[y+1], edgeRight[y]));
        if (flag1){
            checkBottomMapTansition(uPos);
            return;
        }
        if (flag2||flag3){
            return;
        }
        uPos->Y++;
    }
    else{
        return;
    }
    // 判断是否走到障碍物上了
    for (int i = 0; i < barrier.size(); i++) {
        //存在x上的1距离的偏差
        if (barrier[i].X - 1 == uPos->X && barrier[i].Y == uPos->Y){
            *uPos = resetPos;
            return;
        }
    }
}

// 打印屏幕中玩家位置
void Map::print(COORD &uPos, char playerChar) {
    SetConsoleCursorPosition(hOut, uPos);
    putchar(playerChar);
}

//清除屏幕中玩家位置
void Map::clean(COORD clPos) {
    SetConsoleCursorPosition(Map::hOut, clPos);
    putchar(' ');
}

void Map::gotoxy(COORD pos) {
    SetConsoleCursorPosition(hOut, Pos);
}

void Map::checkEvent(COORD &uPos, Player& player, Monster &monster, NPC &npc) {
    // 十字位置按下空格都可以触发
    for (int i = 0; i < barrier.size(); i++) {
        bool flag1 = (uPos.X == (barrier[i].X - 1) && (uPos.Y == barrier[i].Y - 1|| uPos.Y == barrier[i].Y + 1));
        bool flag2 = ((uPos.Y == barrier[i].Y) && (uPos.X == barrier[i].X - 2 || uPos.X == barrier[i].X));
        if (flag1||flag2){
            // 判断事件,调用不同的接口
            MessageBox(nullptr, "事件发生", "提示", MB_OK);
        }
    }
}
#endif //GAME_MAP_H
