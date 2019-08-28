#include <iostream>
#include "/src/GameLoop.h"
#include "/src/global.h"
string mapPath = R"(../data/map.txt)";
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
Player player;
vector<NPC>globalNPC;
vector<Monster>globalMonster;
vector<int>bannedCommand;
SCOORD uPos = {1, 1};
int main() {
    // 做一些检查工作
    std::cout << "Hello, World!" << std::endl;
    return 0;
}