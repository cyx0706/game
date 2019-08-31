
#include "src/GameLoop.h"
#include "src/Package.h"
#include "src/Character.h"
#include "src/Client.h"
#include "src/Status.h"
#include "src/global.h"
#include "src/Map.h"

Client client;
//Package playerPackage;
//Armor armor;
//Status playerStatus;
//Weapon weapon;
Player player;
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;

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
//    std::cout << "Hello, World!" << std::endl;
    GameLoop::initGame();
    GameLoop::mapLoop();
    return 0;
}