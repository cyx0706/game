
#include "src/GameLoop.h"
#include "src/Package.h"
#include "src/Character.h"
#include "src/Client.h"
#include "src/Status.h"
#include "src/global.h"
#include "src/Map.h"

Client client;
Player player;
HANDLE hOut;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
unique_ptr<Map>mapNow;
//vector<NPC>globalNPC(16);
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