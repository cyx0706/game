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
 * @brief ��ʼ����ͼ
 */
void Map::initMap() {
    setCursorStatus(false);
    initChar();
    short temp = 0;
    bool flag = false;
    SCOORD pos = {0, 0};
    gotoxy(pos);
    for (int j = edgeLeft[0]; j <= edgeRight[0]; j++) {
        // ���ϱ߽����
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
            temp = i+1;  // ֱ��дi+1�������Ҫһ���м����
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
    temp = edgeLeft.size() -1; // �޷�ֱ��ʹ�ñ��ʽ,������ʱ����
    pos = {edgeLeft[temp], temp};
    gotoxy(pos);

    flag = false;
    for (int j = edgeLeft[temp]; j <= edgeRight[temp]; j++) {
        // ���±߽����
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
 * @brief ��ʼ���ϰ���
 */
void Map::initBarrier() {
    //��һ��ӳ��Ķ�Ӧ����,��ͬ�ķ��ű�ʾ
    SetConsoleTextAttribute(hOut, 0x0a);
    GetConsoleScreenBufferInfo(hOut, &screenInfo);
    short xLeft = 0;
    short xRight = 0;
    short y = 0;
    // ��NPC��Monster
    for (auto iter= this->barrier.begin();  iter!=this->barrier.end() ; iter++) {
        CHAR_INFO chFill;   //���������������ַ�
        auto m = monsters.find(*iter);
        if(m!=monsters.end()){
            // ��Monster
            chFill = {'M',  screenInfo.wAttributes};
        }
        auto itemFinder = items.find(*iter);
        if (itemFinder != items.end()){
            // ��Item
            chFill = {'I', screenInfo.wAttributes};
        }
        auto npcFinder = npcs.find(*iter);
        if (npcFinder != end(npcs)){
            // ��npc
            chFill = {'N',  screenInfo.wAttributes}; //���������������ַ�
        }
        xLeft = short((*iter).X - 1);
        xRight = short((*iter).X);
        y = short((*iter).Y);
        SMALL_RECT CutScr = {xLeft, y, xRight, y};
        COORD cutPos = {xRight, y};
        ScrollConsoleScreenBuffer(hOut, &CutScr, nullptr, cutPos, &chFill); //�ƶ��ı�
    }
    SetConsoleTextAttribute(hOut, 0x0f);
}


/*
 * @brief ��ʼ���û���λ��
 *
 * @param playerChar:���������û����ַ�
 */
void Map::initChar(char playerChar/* ='P '*/) {
    SetConsoleCursorPosition(hOut, initPos);
    putchar(playerChar);
}

/*
 * @brief ������һ�ŵ�ͼ
 *
 * @param mapId:Ҫ���ɵ�ͼ��id
 * @return ������һ�ŵ�ͼ��ָ��
 */
void Map::nextMap(int mapId) {
    SCOORD pos = this->road[uPos];
    mapNow = make_unique<Map>();
    mapNow->load(mapId);          // ��ȡ��ͼ
    mapNow->initPos = pos; // ���ó����ص�
}

/*
 * @brief �������Ƿ񵽴��˵�ͼ�ײ��ת�Ƶ�
 *
 * @param ��ҵ�����
 * @return �Ƿ񵽴�
 */
bool Map::checkBottomMapTransition() {
    for (int i = 0; i < doorPosBottom.size(); i++) {
        if (uPos.X == doorPosBottom[i]){
            int t = mapNow->roadTo[uPos];
            // ���ɽ���
            if(!canEnter(t, this->id)){
                string tips = "���ڻ����ܽ���";
                char strPtr[50];
                Tool::stringToChar(tips, strPtr);
                MessageBox(nullptr, strPtr, "��ʾ", MB_OK);
                return false;
            }
            system("cls");
            cout << "Waiting......";
            Sleep(500);
            system("cls");
            mapNow->nextMap(t);
            // ���Ƶ�ͼ
            mapNow->initMap();
            uPos.X = mapNow->initPos.X;
            uPos.Y = mapNow->initPos.Y;
            return true;
        }
    }
    return false;
}


/*
 * @brief �������Ƿ񵽴��˵�ͼ������ת�Ƶ�
 *
 * @param uPos��ҵ����������
 * @return �Ƿ񵽴�
 */
bool Map::checkTopMapTransition() {
    for (int i = 0; i < doorPosTop.size(); i++) {
        if (uPos.X == doorPosTop[i]){
            // ����ָ�벢��ȡ����
            int t = mapNow->roadTo[uPos];
            if (mapNow->checkSpecialScene(t)){
                return true;
            }
            if (!Map::canEnter(t, this->id)){
                string tips = "���ڻ����ܽ���";
                char strPtr[50];
                Tool::stringToChar(tips, strPtr);
                MessageBox(nullptr, strPtr, "��ʾ", MB_OK);
                return false;
            }
            system("cls");
            cout << "Waiting......";
            Sleep(500);
            system("cls");
            mapNow->nextMap(t);
            // ���Ƶ�ͼ
            mapNow->initMap();
            // �޸����λ��
            uPos.X = mapNow->initPos.X;
            uPos.Y = mapNow->initPos.Y;
            return true;
        }
    }
    return false;
}

/*
 * @brief ����id�����ļ��������ͼ��Ϣ
 *
 * @param mapId: Ҫ��ȡ��ͼ��id
 */
void Map::load(int mapId) {
    ifstream fp;
    unsigned int index;
    int id;
    vector<string>temp;
    string sentence;
    //��������ƥ���
    regex r;
    r = R"(\{(\d+),(\d+)\})";
    smatch group;
    fp.open(MAP_TXT_PATH);
    for (int turn = 0; turn < (mapId-1)*14; turn++) {
        getline(fp, sentence);
    }
    while (fp.peek() != EOF){
        getline(fp, sentence); // �ļ�������һ��
       if (sentence == "---"){
           getline(fp, sentence); // ����id
           index = sentence.find_first_of(' ') + 1; // ���һλ
           // ASCII�ַ���int�Ͳ�48
           id = int(sentence[index]) - 48;
           if (mapId == id){
               // �����������
               mapNow->id = id;
               // ��������
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->nameCN = sentence.substr(index);
               // ��Ӣ����
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->nameEN = sentence.substr(index);
               //���߽����
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               mapNow->edgeSign = char(sentence[index]);

               //��ȡ��ʼλ��,��������趨,��������ν
               getline(fp, sentence);
               mapNow->initPos = {1, 1};

               //��ȡ��߽�
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               sentence = sentence.substr(index);
               temp = Tool::split(sentence, ',');
               for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                   mapNow->edgeLeft.push_back(fromString<short>(*iter)); // stringתint
               }

               // ��ȡ�ұ߽�
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               sentence = sentence.substr(index);
               temp = Tool::split(sentence, ',');
               for (auto iter = temp.begin(); iter != temp.end(); iter++) {
                   mapNow->edgeRight.push_back(fromString<short>(*iter)); // stringתint
               }
               string foo;
               // ��ȡ������
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index); // ��ȡ�������ַ���
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
               //��ȡ�ײ���
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index); // ��ȡ�������ַ���
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
               // ��ȡ�ϰ���
               getline(fp, sentence);
               index = sentence.find_first_of(' ') + 1;
               foo = sentence.substr(index);
               temp = Tool::split(foo, ' ');
               for (auto iter = temp.begin(); iter < temp.end(); iter++) {
                   regex_match(*iter, group, r);
                   SCOORD pos = {short(stoi(group[1])), short(stoi(group[2]))};
                   mapNow->barrier.push_back(pos);
               }
               //npc monsterӳ���ƥ������ʽ
               regex mapRegex;
               mapRegex = R"((.{5}):\{(\d+),(\d+)\})";

               //��ȡ����
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
               //��ȡnpc
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
               //��ȡitem
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
    // �޳�����ʾ��npc
    // Ҫȷ��npcλ�ú͵�ͼ�б����һ��
    for (auto iter = globalNPC.begin(); iter != globalNPC.end(); iter++) {
        if (iter->mapLocation.mapId == this->id && !(iter->getVisibility())){
            // ɾ�����npc
            SCOORD npcPos = {short(iter->mapLocation.x), short(iter->mapLocation.y)};
            for (auto i = barrier.begin(); i != barrier.end(); i++) {
                if (*i == npcPos){
                    // һ��ɾһ��
                    barrier.erase(i);
                    break;
                }
            }
        }
    }
}

/*
 * @brief �����û��ƶ��ĺ���
 *
 * @param key:�����
 */
void Map::move(int key) {
    SCOORD resetPos = uPos;
    short x = uPos.X;
    short y = uPos.Y;
    //������
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
    //������
    else if (key == 75){
        if (x > edgeLeft[y] + 1){
            uPos.X--;
        }
        else{
            return;
        }

    }
    //������
    else if (key == 77){
        if (x < edgeRight[y] - 1)
            uPos.X++;
        else{
            return;
        }
    }
    //������
    else if (key == 80){
        // 20��С�ĵ�ͼ,����ߵ�18
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
    // �ж��Ƿ��ߵ��ϰ�������
    for (int i = 0; i < barrier.size(); i++) {
        //����x�ϵ�1�����ƫ��
        if (barrier[i].X - 1 == uPos.X && barrier[i].Y == uPos.Y){
            uPos = resetPos;
            for (auto iter = monsters.begin(); iter != monsters.end(); iter++) {
                // �ߵ��˹�������,ֱ��ս��
                if (iter->first == barrier[i]){
                    if (monsters[barrier[i]] == "NY-04"){
                        if (player.getItem(301)){
                            char t[60];
                            string tips = "�������ڳ�˯, ���ǲ�Ҫ��������";
                            Tool::stringToChar(tips, t);
                            MessageBox(nullptr, t, "��ʾ", MB_OK);
                            return;
                        }
                    }
                    Monster aMonster(monsters[barrier[i]]);
                    string tips = "��" + aMonster.nameCN + "��ս";
                    char t[80];
                    Tool::stringToChar(tips, t);
                    MessageBox(nullptr, t, "��ʾ", MB_OK);
                    GameLoop::battleLoop(aMonster);
                    string type = "monster";
                    // ɾ������
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
 * @brief ��ӡ��Ļ�����λ��
 *
 * @param playerChar:��ҵı�ʾ
 */
void Map::print(char playerChar/* ='P' */) {
    SetConsoleCursorPosition(hOut, uPos);
    putchar(playerChar);
}

/*
 * @brief �����Ļ�еĵ�ǰλ��
 *
 * @param clPos: λ��
 */
void Map::clean(SCOORD clPos) {
    SetConsoleCursorPosition(hOut, clPos);
    putchar(' ');
}

/*
 * @brief ʹ����ƶ���ָ��λ��
 *
 * @param pos:ָ������
 */
void Map::gotoxy(SCOORD pos) {
    SetConsoleCursorPosition(hOut, pos);
}

/*
 * @brief ����¼��������¼�
 * �������������ж����������ʾ
 * @return �����¼�����
 * 0:ս���¼�
 * 1:npc�Ի��¼�
 * 2:�ɼ��¼�
 * 3:���¼�
 */
int Map::checkEvent() {
    // ʮ��λ�ð��¿ո񶼿��Դ���
    for (int i = 0; i < barrier.size(); i++) {
        bool flag1 = (uPos.X == (barrier[i].X - 1) && (uPos.Y == barrier[i].Y - 1|| uPos.Y == barrier[i].Y + 1)); // �ж�����λ��
        bool flag2 = ((uPos.Y == barrier[i].Y) && (uPos.X == barrier[i].X - 2 || uPos.X == barrier[i].X)); // �ж�����λ��
        if (flag1||flag2){
            // �ж��¼�,���ò�ͬ�Ľӿ�
            // �ɼ�
            auto resultItem = items.find(barrier[i]);
            if (resultItem != items.end()){
                int itemId = items[barrier[i]];
                // ����item
                Item pickedItem(itemId, 1);
                player.addItem(itemId, 1);
                string tips = "������Ʒ" + pickedItem.nameCN;
                char t[50];
                Tool::stringToChar(tips, t);
                MessageBox(nullptr, t, "��ʾ", MB_OK);
                return 2;
            }
            // npc�Ի�
            // ʹ��find��ʱ�����
            for (auto resultNPC = npcs.begin(); resultNPC != npcs.end() ; resultNPC++) {
                if (resultNPC->first == barrier[i]){
                    for (auto iter = globalNPC.begin(); iter != globalNPC.end() ; iter++) {
                        if (iter->id == resultNPC->second){
                            string tips = "��" + iter->nameCN + "�Ի�";
                            char t[50];
                            Tool::stringToChar(tips, t);
                            MessageBox(nullptr, t, "��ʾ", MB_OK);
                            if (iter->getShopStatus()){
                                GameLoop::shopLoop(*iter);
                            }
                            else{
                                GameLoop::npcLoop(*iter);
                            }
                            // �ָ���ͼ������ƫ��
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
                        string tips = "�������ڳ�˯, ���ǲ�Ҫ��������";
                        Tool::stringToChar(tips, t);
                        MessageBox(nullptr, t, "��ʾ", MB_OK);
                        return 0;
                    }
                }
                Monster aMonster(monsters[barrier[i]]);
                string tips = "��" + aMonster.nameCN + "��ս";
                char t[80];
                Tool::stringToChar(tips, t);
                MessageBox(nullptr, t, "��ʾ", MB_OK);
                GameLoop::battleLoop(aMonster);
                string type = "monster";
                // ɾ������
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
 * @brief չʾ��ͼ����Ϣ
 */
void Map::showDescription() {
    cout << this->nameCN << " : " << this->nameEN << endl;
}

/*
 * @brief ��������¼�,���ھ����趨
 *
 * @return �����Ƿ����������Ľ����������
 * true:����
 *
 */
bool Map::checkSpecialScene(int toMapId) {
    if (toMapId != 1 && toMapId != 2){
        return false;
    }
    // ������ί��
    // �ʹ��˾��ս
    if(player.getMission(11)){
        if (toMapId == 1){
            GameLoop::battleLoop(globalNPC[15]);
            return true;

        }
        if (toMapId == 2){
            system("cls");
            cout << "Ҫ�Ͽ�ȥ��ֹ���˾" << endl;
            return false;
        }
    }
    return false;
}

/*
 * @brief �ж��Ƿ���Խ����ͼ��
 * ��Ҫ��ȡplayer���������
 * @param mapId:��ͼ��id
 * @return true:���Խ���
 */
bool Map::canEnter(int mapId, int fromMapId) {
    // ����2
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
    // ����1
    else{
        if (mapId == 4){
            if (!player.getMission(2)){
                return false;
            }
        }
        if (mapId == 6){
            // �ж�����8�Ƿ��Ѿ�����
            if (!player.getMission(8)){
                return false;
            }
        }
        if (mapId == 7){
            // �ж�����3�Ƿ����
            if (!player.getMission(3)){
                return false;
            }
            // �ڶ�����Ĭ�Ͽ���
        }
        if (mapId == 8){
            // �ж��Ƿ����������10
            if (!player.getMission(10)){
                return false;
            }
        }
        if (mapId == 9){
            // �ж�����3�Ƿ����
            Mission* missionPtr = player.getMission(3);
            if (missionPtr == nullptr || !(missionPtr->isFinished)){
                return false;
            }
        }
    }
    return true;
}

/*
 * @brief ɾ���ϰ���,�����޸ĵ�ͼ
 */
void Map::deleteBarrier(SCOORD& pos, string &type) {
    // ɾ������ϰ����
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
            // �����ʾ
            SCOORD cleanPos = {short(x - 1), y}; // �и�����,����һ��ƫ��
            clean(cleanPos);
            return;
        }
    }
}

/*
 * @brief ���ù���״̬
 *
 * @param �Ƿ���ʾ��� true:��ʾ
 */
void Map::setCursorStatus(bool visible) {
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
