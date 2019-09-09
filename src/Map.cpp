//
// Created by cyx on 2019/8/27.
//
#include "Map.h"

#include <fstream>
#include <regex>
#include <memory>
#include <algorithm>
#include <istream>

#include "GameLoop.h"
#include "Tool.h"
#include "templateHeader.h"
#include "Character.h"
#include "Scene.h"
#include "global.h"
extern unique_ptr<Map>mapNow;
extern HANDLE hOut;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern SCOORD uPos;
extern Player player;
extern vector<NPC>globalNPC;
extern bool returnToMain;

#define MAP_TXT_PATH "../data/map.txt"

bool SCOORD::operator<(const SCOORD &pos) {
    return ((this->X < pos.X) || (this->Y < pos.Y));
}
bool SCOORD::operator>(const SCOORD &pos) {
    return !(this->operator<(pos));
}
bool SCOORD::operator>(const SCOORD &pos) const {
    return !(this->operator<(pos));
}
bool SCOORD::operator<(const SCOORD &pos) const {
    return this->X < pos.X || this->Y < pos.Y;
}
bool SCOORD::operator==(const SCOORD &pos) {
    return this->X == pos.X && this->Y == pos.Y;
}
bool SCOORD::operator==(const SCOORD &pos) const {
    return this->X == pos.X && this->Y == pos.Y;
}


/*
 * @brief 初始化地图
 */
void Map::initMap() {
    setCursorStatus(false);
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
    cout << endl;
    cout << "         " << this->nameCN << endl;
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
    // 画NPC和Monster
    for (auto iter= this->barrier.begin();  iter!=this->barrier.end() ; iter++) {
        CHAR_INFO chFill;   //定义剪切区域填充字符
        auto m = monsters.find(*iter);
        if(m!=monsters.end()){
            // 画Monster
            chFill = {'M',  screenInfo.wAttributes};
        }
        auto itemFinder = items.find(*iter);
        if (itemFinder != items.end()){
            // 画Item
            chFill = {'I', screenInfo.wAttributes};
        }
        auto npcFinder = npcs.find(*iter);
        if (npcFinder != end(npcs)){
            // 画npc
            chFill = {'N',  screenInfo.wAttributes}; //定义剪切区域填充字符
        }
        xLeft = short((*iter).X - 1);
        xRight = short((*iter).X);
        y = short((*iter).Y);
        SMALL_RECT CutScr = {xLeft, y, xRight, y};
        COORD cutPos = {xRight, y};
        ScrollConsoleScreenBuffer(hOut, &CutScr, nullptr, cutPos, &chFill); //移动文本
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
void Map::nextMap(int mapId) {
    SCOORD pos = this->road[uPos];
    mapNow = make_unique<Map>();
    mapNow->load(mapId);          // 读取地图
    mapNow->initPos = pos; // 设置出生地点
}

/*
 * @brief 检查玩家是否到达了地图底侧的转移点
 *
 * @param 玩家的坐标
 * @return 是否到达
 */
bool Map::checkBottomMapTransition() {
    for (int i = 0; i < doorPosBottom.size(); i++) {
        if (uPos.X == doorPosBottom[i]){
            int t = mapNow->roadTo[uPos];
            // 不可进入
            if(!canEnter(t, this->id)){
                string tips = "现在还不能进入";
                char strPtr[50];
                Tool::stringToChar(tips, strPtr);
                MessageBox(nullptr, strPtr, "提示", MB_OK);
                return false;
            }
            system("cls");
            cout << "Waiting......";
            Sleep(500);
            system("cls");
            mapNow->nextMap(t);
            // 绘制地图
            mapNow->initMap();
            uPos.X = mapNow->initPos.X;
            uPos.Y = mapNow->initPos.Y;
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
    for (int i = 0; i < doorPosTop.size(); i++) {
        if (uPos.X == doorPosTop[i]){
            // 生成指针并读取数据
            int t = mapNow->roadTo[uPos];
            if (mapNow->checkSpecialScene(t)){
                return true;
            }
            if (!Map::canEnter(t, this->id)){
                string tips = "现在还不能进入";
                char strPtr[50];
                Tool::stringToChar(tips, strPtr);
                MessageBox(nullptr, strPtr, "提示", MB_OK);
                return false;
            }
            system("cls");
            cout << "Waiting......";
            Sleep(500);
            system("cls");
            mapNow->nextMap(t);
            // 绘制地图
            mapNow->initMap();
            // 修改玩家位置
            uPos.X = mapNow->initPos.X;
            uPos.Y = mapNow->initPos.Y;
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
    r = R"(\{(\d+),(\d+)\})";
    smatch group;
    fp.open(MAP_TXT_PATH);
    for (int turn = 0; turn < (mapId-1)*14; turn++) {
        getline(fp, sentence);
    }
    while (fp.peek() != EOF){
        getline(fp, sentence); // 文件流读入一行
       if (sentence == "---"){
           getline(fp, sentence); // 读出id
           index = sentence.find_first_of(' ') + 1; // 向后一位
           // ASCII字符和int型差48
           id = int(sentence[index]) - 48;
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
               index = sentence.find_first_of(' ') + 1;
               sentence = sentence.substr(index);
               temp = Tool::split(sentence, ',');
               for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                   mapNow->edgeLeft.push_back(fromString<short>(*iter)); // string转int
               }

               // 读取右边界
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               sentence = sentence.substr(index);
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
               DoorRegex = R"((\d+):(\d+):\{(\d+),(\d+)\})";
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
                   temp = Tool::split(foo, ' ');
                   for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                       regex_match(*iter, group, DoorRegex);
                       short door = short(stoi(group[1]));
                       int nextMapId = stoi(group[2]);
                       SCOORD pos = {short(stoi(group[3])), short(stoi(group[4]))};
                       mapNow->roadTo.insert(make_pair(SCOORD {door, 18}, nextMapId));
                       mapNow->road.insert(make_pair(SCOORD {door, 18}, pos));
                       mapNow->doorPosBottom.push_back(door);
                   }
               }
               // 读取障碍物
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               temp = Tool::split(foo, ' ');
               for (auto iter = temp.begin(); iter < temp.end(); iter++) {
                   regex_match(*iter, group, r);
                   SCOORD pos = {short(stoi(group[1])), short(stoi(group[2]))};
                   mapNow->barrier.push_back(pos);
               }
               //npc monster映射的匹配正则式
               regex mapRegex;
               mapRegex = R"((.{5}):\{(\d+),(\d+)\})";

               //读取怪物
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               if (foo != "None"){
                   temp = Tool::split(foo, ' ');
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
                   temp = Tool::split(foo, ' ');
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
               if (foo != "None"){
                   regex itemRegex;
                   itemRegex = R"((\d+):\{(\d+),(\d+)\})";
                   temp = Tool::split(foo, ' ');
                   for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                       regex_match(*iter, group, itemRegex);
                       SCOORD pos = {short(stoi(group[2])), short(stoi(group[3]))};
                       int itemId = stoi(group[1]);
                       mapNow->items.insert(make_pair(pos, itemId));
                   }
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
    fp.close();
    // 剔除不显示的npc
    // 要确保npc位置和地图中保存的一致
    for (auto iter = globalNPC.begin(); iter != globalNPC.end(); iter++) {
        if (iter->mapLocation.mapId == this->id && !(iter->getVisibility())){
            // 删除这个npc
            SCOORD npcPos = {short(iter->mapLocation.x), short(iter->mapLocation.y)};
            for (auto i = barrier.begin(); i != barrier.end(); i++) {
                if (*i == npcPos){
                    // 一次删一个
                    barrier.erase(i);
                    break;
                }
            }
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
            for (auto iter = monsters.begin(); iter != monsters.end(); iter++) {
                // 走到了怪物身上,直接战斗
                if (iter->first == barrier[i]){
                    if (monsters[barrier[i]] == "NY-04"){
                        if (player.getItem(301)){
                            char t[60];
                            string tips = "巨龙正在沉睡, 还是不要打扰它了";
                            Tool::stringToChar(tips, t);
                            MessageBox(nullptr, t, "提示", MB_OK);
                            return;
                        }
                    }
                    Monster aMonster(monsters[barrier[i]]);
                    string tips = "和" + aMonster.nameCN + "对战";
                    char t[80];
                    Tool::stringToChar(tips, t);
                    MessageBox(nullptr, t, "提示", MB_OK);
                    GameLoop::battleLoop(aMonster);
                    string type = "monster";
                    // 删除怪物
                    mapNow->deleteBarrier(barrier[i], type);
                    mapNow->initPos = uPos;
                    mapNow->initMap();
                    return;
                }
            }
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
 * @brief 检查事件并触发事件
 * 在主调函数处判断消除点的显示
 * @return 返回事件类型
 * 0:战斗事件
 * 1:npc对话事件
 * 2:采集事件
 * 3:无事件
 */
int Map::checkEvent() {
    // 十字位置按下空格都可以触发
    for (int i = 0; i < barrier.size(); i++) {
        bool flag1 = (uPos.X == (barrier[i].X - 1) && (uPos.Y == barrier[i].Y - 1|| uPos.Y == barrier[i].Y + 1)); // 判断上下位置
        bool flag2 = ((uPos.Y == barrier[i].Y) && (uPos.X == barrier[i].X - 2 || uPos.X == barrier[i].X)); // 判断左右位置
        if (flag1||flag2){
            // 判断事件,调用不同的接口
            // 采集
            auto resultItem = items.find(barrier[i]);
            if (resultItem != items.end()){
                int itemId = items[barrier[i]];
                // 构造item
                Item pickedItem(itemId, 1);
                player.addItem(itemId, 1);
                string tips = "发现物品" + pickedItem.nameCN;
                char t[50];
                Tool::stringToChar(tips, t);
                MessageBox(nullptr, t, "提示", MB_OK);
                return 2;
            }
            // npc对话
            // 使用find有时会出错
            for (auto resultNPC = npcs.begin(); resultNPC != npcs.end() ; resultNPC++) {
                if (resultNPC->first == barrier[i]){
                    for (auto iter = globalNPC.begin(); iter != globalNPC.end() ; iter++) {
                        if (iter->id == resultNPC->second){
                            string tips = "和" + iter->nameCN + "对话";
                            char t[50];
                            Tool::stringToChar(tips, t);
                            MessageBox(nullptr, t, "提示", MB_OK);
                            if (iter->getShopStatus()){
                                GameLoop::shopLoop(*iter);
                            }
                            else{
                                GameLoop::npcLoop(*iter);
                            }
                            // 恢复地图并修正偏差
                            mapNow->initMap();
                            mapNow->clean(mapNow->initPos);
                            mapNow->gotoxy(uPos);
                            break;
                        }
                    }
                    return 1;
                }
                else{
                    continue;
                }
            }
            auto resultMonster = monsters.find(barrier[i]);
            if (resultMonster != monsters.end()){
                if (monsters[barrier[i]] == "NY-04"){
                    if (player.getItem(301)){
                        char t[60];
                        string tips = "巨龙正在沉睡, 还是不要打扰它了";
                        Tool::stringToChar(tips, t);
                        MessageBox(nullptr, t, "提示", MB_OK);
                        return 0;
                    }
                }
                Monster aMonster(monsters[barrier[i]]);
                string tips = "和" + aMonster.nameCN + "对战";
                char t[80];
                Tool::stringToChar(tips, t);
                MessageBox(nullptr, t, "提示", MB_OK);
                GameLoop::battleLoop(aMonster);
                string type = "monster";
                // 删除怪物
                mapNow->deleteBarrier(barrier[i], type);
                mapNow->initPos = uPos;
                mapNow->initMap();
                return 0;
            }
        }
    }
    return 3;
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
 * @return 返回是否跳过后续的交互进入界面
 * true:跳过
 *
 */
bool Map::checkSpecialScene(int toMapId) {
    if (toMapId != 1 && toMapId != 2){
        return false;
    }
    // 公主的委托
    // 和大祭司对战
    if(player.getMission(11)){
        if (toMapId == 1){
            GameLoop::battleLoop(globalNPC[15]);
            return true;

        }
        if (toMapId == 2){
            system("cls");
            cout << "要赶快去阻止大祭司" << endl;
            return false;
        }
    }
    return false;
}

/*
 * @brief 判断是否可以进入地图中
 * 需要获取player的任务进度
 * @param mapId:地图的id
 * @return true:可以进入
 */
bool Map::canEnter(int mapId, int fromMapId) {
    // 主线2
    if (player.getItem(301)){
        if (mapId == 8){
            if (!player.getMission(10)){
                return false;
            }
        }
        if (mapId == 6){
            if (!player.getMission(8)){
                return false;
            }
        }
        if (mapId == 9 && fromMapId == 7){
            return false;
        }
        if (mapId == 7 && fromMapId == 9){
            return false;
        }
        if (mapId == 1){
            return false;
        }
    }
    // 主线1
    else{
        if (mapId == 4){
            if (!player.getMission(2)){
                return false;
            }
        }
        if (mapId == 6){
            // 判断任务8是否已经接受
            if (!player.getMission(8)){
                return false;
            }
        }
        if (mapId == 7){
            // 判断任务3是否接受
            if (!player.getMission(3)){
                return false;
            }
            // 第二条线默认开放
        }
        if (mapId == 8){
            // 判断是否接受了任务10
            if (!player.getMission(10)){
                return false;
            }
        }
        if (mapId == 9){
            // 判断任务3是否完成
            Mission* missionPtr = player.getMission(3);
            if (missionPtr == nullptr || !(missionPtr->isFinished)){
                return false;
            }
        }
    }
    return true;
}

/*
 * @brief 删除障碍物,用于修改地图
 */
void Map::deleteBarrier(SCOORD& pos, string &type) {
    // 删除这个障碍物点
    for (auto iter = this->barrier.begin(); iter != this->barrier.end(); iter++) {
        short x = (*iter).X;
        short y = (*iter).Y;
        bool flag1 = (uPos.X == (x - 1) && (uPos.Y == y - 1|| uPos.Y == y + 1));
        bool flag2 = ((uPos.Y == y) && (uPos.X == x - 2 || uPos.X == x));
        if (flag1||flag2){
            barrier.erase(iter);
            if (type == "item"){
                this->items.erase(*iter);
            }
            else if (type == "monster"){
                this->monsters.erase(*iter);
            }
            else if (type == "npc"){
                this->npcs.erase(*iter);
            }
            else{
                return;
            }
            // 清除显示
            SCOORD cleanPos = {short(x - 1), y}; // 切割问题,存在一格偏差
            clean(cleanPos);
            return;
        }
    }
}

/*
 * @brief 设置光标的状态
 *
 * @param 是否显示光标 true:显示
 */
void Map::setCursorStatus(bool visible) {
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
