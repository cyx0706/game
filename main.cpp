
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
vector<NPC>globalNPC;
SCOORD uPos = {1, 1};
Shop NPC::store = Shop();
int main() {
    // ��һЩ��鹤��
    // ���������
    srand(static_cast<unsigned int>(time(nullptr)));
    GameLoop::initGame();
    GameLoop::mapLoop();
    return 0;
}