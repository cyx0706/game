//
// Created by cyx on 2019/8/27.
//
#include "Map.h"

/*
 * @brief 初始化地图
 */
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


/*
 * @brief 初始化障碍物
 */
void Map::initBarrier() {
    //查一下映射的对应对象,不同的符号表示
    SetConsoleTextAttribute(hOut, 0x0a);
    GetConsoleScreenBufferInfo(hOut, &screenInfo);
    CHAR_INFO chFill = {'T',  screenInfo.wAttributes}; //定义剪切区域填充字符
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


/*
 * @brief 初始化用户的位置
 *
 * @param playerChar:用来代表用户的字符
 */
void Map::initChar(char playerChar/* ='P '*/) {
    SetConsoleCursorPosition(hOut, initPos);
    putchar(playerChar);
}

/*
 * @brief 生成下一张地图
 *
 * @param mapId:要生成地图的id
 * @return 返回下一张地图的指针
 */
unique_ptr<Map> Map::nextMap(int mapId) {
    auto mapNext = make_unique<Map>();
    mapNext->load(mapId);          // 读取地图
    mapNext->initPos = road[uPos]; // 设置出生地点
    return mapNext;
}

/*
 * @brief 检查玩家是否到达了地图底侧的转移点
 *
 * @param 玩家的坐标
 * @return 是否到达
 */
bool Map::checkBottomMapTransition() {
    for (int i = 0; i < doorPosTop.size(); i++) {
        if (uPos.Y == doorPosTop[i]){
            return true;
        }
    }
}


/*
 * @brief 检查玩家是否到达了地图顶部的转移点
 *
 * @param uPos玩家的坐标的引用
 * @return 是否到达
 */
bool Map::checkTopMapTransition() {
    for (int i = 0; i < doorPosBottom.size(); i++) {
        if (uPos.X == doorPosBottom[i]){
            return true;
        }
    }
}

/*
 * @brief 根据id来在文件里读出地图信息
 *
 * @param mapId: 要读取地图的id
 */
void Map::load(int mapId) {

}

/*
 * @brief 控制用户移动的函数
 *
 * @param uPos:玩家坐标的引用 key:方向键
 */
void Map::move(int key) {
    COORD resetPos = uPos;
    short x = uPos.X;
    short y = uPos.Y;
    //向上走
    if(key == 72){
        bool flag1 = (y <= 1) ;
        bool flag2 = (x >= min(edgeLeft[y-1], edgeLeft[y]) && x <= max(edgeLeft[y-1], edgeLeft[y]));
        bool flag3 = (x >= min(edgeRight[y-1], edgeRight[y]) && x <= max(edgeRight[y-1], edgeRight[y]));
        if (flag1){
            checkTopMapTransition();
            return;
        }
        if (flag2||flag3){
            return;
        }
        uPos.Y--;
    }
    //向左走
    else if (key == 75){
        if (x > edgeLeft[y] + 1){
            uPos.X--;
        }
        else{
            return;
        }

    }
    //向右走
    else if (key == 77){
        if (x < edgeRight[y] - 1)
            uPos.X++;
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
            checkBottomMapTransition();
            return;
        }
        if (flag2||flag3){
            return;
        }
        uPos.Y++;
    }
    else{
        return;
    }
    // 判断是否走到障碍物上了
    for (int i = 0; i < barrier.size(); i++) {
        //存在x上的1距离的偏差
        if (barrier[i].X - 1 == uPos.X && barrier[i].Y == uPos.Y){
            uPos = resetPos;
            return;
        }
    }
}

/*
 * @brief 打印屏幕中玩家位置
 *
 * @param playerChar:玩家的表示
 */
void Map::print(char playerChar/* ='P' */) {
    SetConsoleCursorPosition(hOut, uPos);
    putchar(playerChar);
}

/*
 * @brief 清除屏幕中的当前位置
 *
 * @param clPos: 位置
 */
void Map::clean(COORD clPos) {
    SetConsoleCursorPosition(hOut, clPos);
    putchar(' ');
}

/*
 * @brief 使光标移动到指定位置
 *
 * @param pos:指定坐标
 */
void Map::gotoxy(COORD pos) {
    SetConsoleCursorPosition(hOut, pos);
}

/*
 * @brief 检查事件
 *
 * @return 返回事件类型
 */
int Map::checkEvent() {
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

/*
 * @brief 展示地图的信息
 */
void Map::showDescription() {
    cout << this->nameCN << " : " << this->nameEN << endl;
}

/*
 * @brief 检查特殊事件,用于剧情设定
 *
 */

void Map::checkSpecialScene() {

}


