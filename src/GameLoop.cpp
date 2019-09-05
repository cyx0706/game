//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
#include "Map.h"
#include "Tool.h"
#include "Scene.h"
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "Status.h"
#include "Character.h"
#include "global.h"
#include "Client.h"

extern HANDLE hOut;
using CMD::CommandLists;
extern Client client;
extern Player player;
extern unique_ptr<Map>mapNow;
extern vector<NPC>globalNPC;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern SCOORD uPos;
extern vector<string>npcName;
/*
 * @brief ��Ϸ�ĵ�ͼѭ��
 * ��Ҫ��ʼ����ɺ���ܵ���
 */
void GameLoop::mapLoop() {
    GetConsoleCursorInfo(hOut, &cursorInfo); //��ȡ����̨�����Ϣ
    cursorInfo.bVisible = false;             //���ؿ���̨���
    SetConsoleCursorInfo(hOut, &cursorInfo); //���ÿ���̨���״̬
    mapNow->initMap();
    mapNow->print();
    char input;
    while(true){
        if(kbhit()){
            mapNow->clean(uPos);//���ԭ�����
            input = getch();
            // ����Escת��Ϊ������ģʽ,����true֪ͨ��������
            if (input == ESC){
                system("cls"); // �����Ļ
                commandLoop();
                mapNow->initPos = uPos;
                mapNow->initMap();
            }
            if(input == SPACE){
                int branch = mapNow->checkEvent();
                if (branch == 2){
                    string barrierType = "item";
                    mapNow->deleteBarrier(uPos, barrierType);
                }
                if (branch == 0){
                    string barrierType = "monster";
//                    mapNow->deleteBarrier(uPos, barrierType);
                }
                mapNow->clean(uPos);
                mapNow->print();
            }
            else{
                mapNow->move(input);
                mapNow->print();
            }
        }
    }
}

/*
 * @brief ������ѭ��
 * ����esc�����л�,��mapLoop�ﱻ����
 */
void GameLoop::commandLoop() {
    player.playerMenu();
    Map::setCursorStatus(true);
    client.base();
    Map::setCursorStatus(false);
    system("cls");
}

// ��ʼ��
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    NPC::readLastLine = 0; // ��ʼ��
    mapNow = make_unique<Map>();
    mapNow->load(1);
    Skill skill1("A01");
    player.skills.push_back(skill1);
    Skill skill2("B01");
    player.skills.push_back(skill2);
    Skill skill3("H01");
    player.skills.push_back(skill3);
    Skill skill4("H02");
    player.skills.push_back(skill4);
    player.maxMP = 100;
    player.maxHP = 150;
    player.addItem(201, 2);
    for (unsigned int i = 0; i < npcName.size(); i++) {
        NPC aNPC(npcName[i]);
        globalNPC.push_back(aNPC);
    }

}



void GameLoop::gameInterface(){
    string title = "��������ʨ�������������";
    auto x = short(screenInfo.dwSize.X - title.length() / 2);
    COORD pos = {x, 5};
    SetConsoleCursorPosition(hOut, pos);
    cout << "+----------------------------+"
         << "|  ���� ����ʨ �������������  |"
         << "+----------------------------+"
         << endl;
    cout << "NewGame      Load      Exit   " << endl;
}

void GameLoop::gameStart() {
    // չʾ��Ϸ�Ľ���
    gameInterface();
    string input;
    while (true){
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::tolower); // ת��Сд
        // ɾ����β�ո�
        if( !input.empty() ){
            input.erase(0,input.find_first_not_of(' '));
            input.erase(input.find_last_not_of(' ') + 1);
        }

        if (input == "newgame"){
            // ��ʾ�����û�������
            cout << "����������û���:\b\b\b\b\b";
            cin >> input;
            player.nameCN = input;
            player.nameEN = "None";

            Scene scene(1);
            ifstream fp;
            fp.open("../src/scene1.txt");
            fp >> scene; // չʾ����
            mapLoop();
        }
        else if (input == "load"){
            // ���ö�ȡ
            mapLoop();
        }
        else if (input == "exit"){
            cout << "Bye!";
            Sleep(1000); // ˯1s
            exit(1);
        }
        else{
            // ��ʾ��Ϣ
            cout << "newgame: �µ���Ϸ" << endl;
            cout << "load: ��ȡ�浵" << endl;
            cout << "exit: �˳�" << endl;
        }
    }

}

void GameLoop::npcLoop(NPC &talkedNPC) {
    system("cls");
    Map::setCursorStatus(true);
    talkedNPC.NPCMenu(player);
    client.npcBase(talkedNPC);
    system("cls");
}
/*
 * @brief ս��ѭ��ֱ��һ����������
 * ��������ս�����߼�����
 * ��ҵ��ж�������ϵͳ�����ӿ�ʵ��
 * �����ж�buff�ĳ�����buff�ļӳɼ���������ϵͳʵ��
 * �������ѭ��������������������
 * �жԷ��������㽱��
 * @param ս������character
 */
void GameLoop::battleLoop(Character &character) {
    bool playerTurn = true;
    bool enemyTurn = false;
    int damage;
    SCOORD curPos;
    system("cls");

    cout << "��������" << endl;
    if (character.status.Speed > player.status.Speed){
        playerTurn = true;
        enemyTurn = false;
        damage = character.status.ATK - player.status.DEF;
        if (damage <= 0){
            damage = 1;
        }
        cout << "���˵����ƹ���, �����" << damage << "���˺�" << endl;
        player.status.HP -= damage;
        if(player.isDead()){
            player.deadScene();
            return;
        }
        else{
            cout << "���ʣ��Ѫ��:" << player.status.HP << endl;
        }
    }
    system("pause");
    system("cls");

    int turn = 0; // �غ���
    while (true){
        turn += 1;

        // ս������
        for (int i = 0; i < 80; i++) {
            cout << "*";
        }
        cout << endl;
        for (short k = 1; k < 20; k++) {
            curPos = {40, k};
            Map::gotoxy(curPos);
            cout << "*";
        }
        curPos = {0, 20};
        Map::gotoxy(curPos);
        for (int i = 0; i < 80; i++) {
            cout << "*";
        }
        // ��Ϣ���
        curPos = {7,1};
        Map::gotoxy(curPos);
        cout << "����:" << character.nameCN << "("  << character.nameEN << ")";
        curPos.Y = 2;
        Map::gotoxy(curPos);
        cout << "-> HP:" << character.status.HP;
        curPos.Y = 3;
        Map::gotoxy(curPos);
        cout << "-> MP:" << character.status.MP;
        curPos.Y = 4;
        Map::gotoxy(curPos);
        cout << "-> ATK:" << character.status.ATK;
        curPos.Y = 5;
        Map::gotoxy(curPos);
        cout << "-> DEF:" << character.status.DEF;
        curPos.Y = 6;
        Map::gotoxy(curPos);
        cout << "--------------------" << endl;
        curPos.Y = 7;
        Map::gotoxy(curPos);
        cout << "���: " << player.nameCN << endl;
        curPos.X = 7;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "BUFF:";
        curPos.Y++;
        Map::gotoxy(curPos);
        for (auto iter = player.buffs.begin(); iter != player.buffs.end();) {
            // ֻ��Ϊ0�Ź���
            if ((*iter).duration != 0){
                (*iter).duration--;
                if (iter->duration == 0){
                    player.deleteBuff((*iter));
                    player.buffs.erase(iter);
                    continue;
                }
                else{
                    cout << iter->name << "\tʣ��" << iter->duration << "�غ�";
                    curPos.Y++;
                    Map::gotoxy(curPos);
                }
            }
            iter++;
        }
        cout << "-> HP:" << player.status.HP;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> MP:" << player.status.MP;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> ATK:" << player.status.ATK;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> DEF:" << player.status.DEF;

        curPos = {50, 1};
        Map::gotoxy(curPos);
        cout << "\t\t��Ҽ���";
        short y = 2;
        if (!player.skills.empty()){
            for (auto j = player.skills.begin(); j <player.skills.end() ; j++) {
                curPos = {48, y++};
                Map::gotoxy(curPos);
                cout << setiosflags(ios_base::left)
                     << setw(16) << j->nameCN
                     << setw(16) << j->nameEN
                     << "����MP:" << j->MP;
            }
        }
        else{
            curPos = {48, y++};
            cout << "\t\t�޼���";
        }

        curPos.Y += 2;
        Map::gotoxy(curPos);
        cout << "\t\t�����Ʒ";
        curPos.Y++;
        player.battleBagShow(curPos);

        curPos = {0, 21};
        Map::gotoxy(curPos);
        cout << "-------------------" << endl;
        cout << "�غ�:" << turn << endl;
        cout << "-------------------" << endl;

        // ǰ���ж�ʹ�غϿ�������
        // ��һغ�
        if (playerTurn){
            cout << "��ҵĻغ�:����ж�" << endl;
            Map::setCursorStatus(true);
            client.base(character);
            // ��һ���ǵ����ж�
            playerTurn = false;
            enemyTurn = true;

            // ���ܵ���
            if (character.isDead()){
                cout << "�������" << character.nameCN << "�����ʤ��" << endl;
                cout << "��þ���" << character.fallingExp << endl;
                cout << "��ý�Ǯ" << character.fallingMoney << endl;
                player.addExp(character.fallingExp);
                player.addMoney(character.fallingMoney);
                // �жϵ�����Ʒ
                auto *monster = dynamic_cast<Monster*>(&character);
                // ת��ʧ��ʱΪ��ָ��
                if(monster){
                   for(auto iter = monster->fallingItem.begin(); iter!=monster->fallingItem.end(); iter++){
                       cout << "���������" << (*iter).nameCN << endl;
                       player.addItem((*iter).id, (*iter).num);
                   }
                }
                system("pause");
                system("cls");
                break;
            }
            if (player.isDead()){
                player.deadScene();
                system("pause");
                exit(1);
            }
        }
        if (enemyTurn){
            int possible = getRandom(1, 100); // ��������
            int fluctuation = getRandom(90, 110); // �˺��Ĳ���
            damage = int(character.status.ATK * fluctuation / 100);
            // ����
            if (possible >= character.status.Critical){
                damage = int(damage * 1.5);
            }
            damage -= player.status.DEF;
            if (damage <= 0){
                damage = 1;
            }
            player.status.HP -= damage;
            cout << character.nameCN << "�������" << damage << "�˺�" << endl;
            if (player.isDead()){
                player.deadScene();
                system("pause");
                exit(1);
            }
            // ��һ��������ж�
            playerTurn = true;
            enemyTurn = false;
        }
        system("pause");
        Map::setCursorStatus(false);
        system("cls");
    }
}




