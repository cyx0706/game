//
// Created by cyx on 2019/8/27.
//
#include "Map.h"
#include "Tool.h"
#include <fstream>
#include <regex>
extern HANDLE hOut;
extern unique_ptr<Map>mapNow;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern vector<NPC>globalNPC;
extern vector<Monster>globalMonster;
/*
 * @brief 初始化地图
 */
void Map::initMap() {
    initChar();
    short temp = 0;
    bool flag = false;
    SCOORD pos = {0, 0};
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
    short xLeft = 0;
    short xRight = 0;
    short y = 0;
    // 画NPC
    CHAR_INFO chFill = {'N',  screenInfo.wAttributes}; //定义剪切区域填充字符
    for (auto iter = globalNPC.begin(); iter != globalNPC.end(); iter++) {
        if (!(*iter).mapLocation.mapId == this.mapId){
            continue;
        }
        else{
            xLeft = (*iter).mapLocation.x - 1;
            xRight = (*iter).mapLocation.x;
            y = (*iter).mapLocation.y;
            SMALL_RECT CutScr = {xLeft, y, xRight, y};
            ScrollConsoleScreenBuffer(hOut, &CutScr, nullptr, y, &chFill); //移动文本
        }
    }
    // 画Monster
    CHAR_INFO chFill = {'M',  screenInfo.wAttributes};
    for (auto iter = globalMonster.begin(); iter != globalMonster.end() ; iter++) {
        if (!(*iter).mapLocation.mapId == this.mapId){
            continue;
        }
        else{
            xLeft = (*iter).mapLocation.x - 1;
            xRight = (*iter).mapLocation.x;
            y = (*iter).mapLocation.y;
            SMALL_RECT CutScr = {xLeft, y, xRight, y};
            ScrollConsoleScreenBuffer(hOut, &CutScr, nullptr, y, &chFill); //移动文本
        }
    }
    SetConsoleTextAttribute(hOut, 0x0f);
    // 画item
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
    return false;
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
    return false;
}

/*
 * @brief 根据id来在文件里读出地图信息
 *
 * @param mapId: 要读取地图的id
 */
void Map::load(int mapId) {
    ifstream fp;
    unsigned int index;
    int id;
    vector<string>temp;
    string sentence;
    //声明正则匹配点
    regex r;
    r = R"(\((\d+),(\d+)\))";
    smatch group;
    fp.open(mapPath);
    while (fp.peek() != EOF){
        getline(fp, sentence); // 文件流读入一行
       if (sentence == "---"){
           getline(fp, sentence); // 读出id
           index = sentence.find_first_of(' ') + 1; // 向后一位
           id = int(sentence[index]);
           if (mapId == id){
               // 读后面的数据
               mapNow->id = id;
               // 读中文名
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->nameCN = sentence.substr(index);
               // 读英文名
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->nameEN = sentence.substr(index);
               //读边界符号
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->edgeSign = char(sentence[index]);
               //读取初始位置,在外面会设定,这里无所谓
               getline(fp, sentence);
               mapNow->initPos = {1, 1};
               //读取左边界
               getline(fp, sentence);
               temp = Tool::split(sentence, ',');
               for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                   mapNow->edgeLeft.push_back(fromString<short>(*iter)); // string转int
               }
               // 读取右边界
               getline(fp, sentence);
               temp = Tool::split(sentence, ',');
               for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                   mapNow->edgeRight.push_back(fromString<short>(*iter)); // string转int
               }
               string foo;
               // 读取顶部门
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index); // 获取后续的字符串
               regex DoorRegex;
               DoorRegex = R"((\d+):(\d+):(\d+)\((\d+),(\d+)\))";
               if(foo != "None"){
                   regex_match(foo, group, DoorRegex);
                   short door = short(stoi(group[1]));
                   int nextMapId = stoi(group[2]);
                   SCOORD pos = {short(stoi(group[3])), short(stoi(group[4]))};
                   mapNow->roadTo.insert(make_pair(SCOORD {door, 1}, nextMapId));
                   mapNow->road.insert(make_pair(SCOORD {door, 1}, pos));
                   mapNow->doorPosTop.push_back(door);
               }
               //读取底部门
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index); // 获取后续的字符串
               if(foo != "None"){
                   vector<string>doors;
                   temp = Tool::split(foo, ' ');
                   for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                       regex_match(foo, group, DoorRegex);
                       short door = short(stoi(group[1]));
                       int nextMapId = stoi(group[2]);
                       SCOORD pos = {short(stoi(group[3])), short(stoi(group[4]))};
                       mapNow->roadTo.insert(make_pair(SCOORD {door, 1}, nextMapId));
                       mapNow->road.insert(make_pair(SCOORD {door, 1}, pos));
                       mapNow->doorPosBottom.push_back(door);
                   }
               }
               // 读取障碍物
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               temp = Tool::split(foo, ',');
               for (auto iter = temp.begin(); iter < temp.end(); iter++) {
                   regex_match(*iter, group, r);
                   SCOORD pos = {short(stoi(group[1])), short(stoi(group[2]))};
                   mapNow->barrier.push_back(pos);
               }

               regex mapRegex;
               mapRegex = R"((.{5}):\((\d+),(\d+)\))";
               //读取怪物
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               if (foo != "None"){
                   temp = Tool::split(foo, ',');
                   for (auto iter = temp.begin(); iter < temp.end(); iter++) {
                       regex_match(*iter, group, mapRegex);
                       SCOORD pos = {short(stoi(group[2])), short(stoi(group[3]))};
                       mapNow->monsters.insert(make_pair(pos, group[1]));
                   }
               }
               //读取npc
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               if (foo != "None"){
                   temp = Tool::split(foo, ',');
                   for (auto iter = temp.begin(); iter < temp.end(); iter++) {
                       regex_match(*iter, group, mapRegex);
                       SCOORD pos = {short(stoi(group[2])), short(stoi(group[3]))};
                       mapNow->npcs.insert(make_pair(pos, group[1]));
                   }
               }
               //读取item
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               if (foo == "None"){

               }
           }
           else{
               continue;
           }
       }
       else{
           continue;
       }
    }
}

/*
 * @brief 控制用户移动的函数
 *
 * @param key:方向键
 */
void Map::move(int key) {
    SCOORD resetPos = uPos;
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
void Map::clean(SCOORD clPos) {
    SetConsoleCursorPosition(hOut, clPos);
    putchar(' ');
}

/*
 * @brief 使光标移动到指定位置
 *
 * @param pos:指定坐标
 */
void Map::gotoxy(SCOORD pos) {
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


