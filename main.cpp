#include <iostream>
#include "/src/GameLoop.h"
#include "/src/global.h"
Client client;
string mapPath = R"(../data/map.txt)";
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
Player player;
vector<NPC>globalNPC;
vector<Monster>globalMonster;
// 命名空间？
vector<int>battleBannedCommands = {sell, status, equipment, package, item, equip, takeoff, discard, talk, maps, help, mission, save};
vector<int>talkBannedCommands = {};
vector<int>commonBannedCommands = {};
vector<int>shopBannedCommands = {};
SCOORD uPos = {1, 1};
int main() {
    // 做一些检查工作
    // 随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    std::cout << "Hello, World!" << std::endl;
    return 0;
}