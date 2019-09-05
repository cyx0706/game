
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
int NPC::readLastLine = 0; // �Ż���ȡ
// NN-05������......
vector<string>npcName = {"NN-01", "NN-02", "NN-03", "NN-04", "NN-06", "NN-07", "NN-08",
                         "NN-09", "NN-10", "NN-11", "NN-12", "NN-13", "NN-14", "NN-15", "NN-16",
                         "NY-01", "NY-02"};
int main() {
    // ��һЩ��鹤��
    // ���������
    srand(static_cast<unsigned int>(time(nullptr)));
    GameLoop::initGame();
    GameLoop::mapLoop();
    return 0;
}