#include <iostream>
#include "/src/GameLoop.h"
#include "/src/Package.h"
#include "/src/Character.h"
#include "/src/global.h"
#include "/src/Client.h"
#include "/src/Status.h"
#include "/src/global.h"
#include "/src/Map.h"


Client client;
string mapPath = R"(../data/map.txt)";
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
Package aPackage();
Armor armor();
Status status1();
Weapon weapon();
Player player(aPackage, armor, weapon, status1);
vector<NPC>globalNPC;
vector<Monster>globalMonster;
vector<int>battleBannedCommands = {};
vector<int>talkBannedCommands = {};
vector<int>commonBannedCommands = {};
vector<int>shopBannedCommands = {};
SCOORD uPos = {1, 1};
int main() {
    // 做一些检查工作
    // 随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    std::cout << "Hello, World!" << std::endl;
    GameLoop::initGame();
    GameLoop::mapLoop();
    return 0;
}