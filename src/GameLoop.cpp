//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
#include "Map.h"
#include "Tool.h"
#include "Scene.h"
#include <algorithm>
#include <fstream>
#include "Status.h"
#include "Character.h"
#include "global.h"
#include "Client.h"

extern HANDLE hOut;
extern vector<int>commonBannedCommands;
using CMD::CommandLists;
extern Client client;
extern Player player;
extern unique_ptr<Map>mapNow;
//extern vector<NPC>globalNPC;
extern CONSOLE_SCREEN_BUFFER_INFO screenInfo;
//extern vector<Monster>globalMonster;
extern CONSOLE_CURSOR_INFO cursorInfo;
extern SCOORD uPos;
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
                break;
            }
            if(input == SPACE){
                int branch = mapNow->checkEvent();
                if (branch == 2){
                    string barrierType = "item";
                    mapNow->deleteBarrier(uPos, barrierType);
                }
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
    string input;
    while (true){
        int index = 0;
        getline(cin, input);
        string s = input;
        if (!s.empty()){
            // ѭ��ɾ�����еĿո�
            while ((index = s.find(' ', index)) != string::npos){
                s.erase(index); // ɾ���ո�
            }
        }
        if (s == "esc"){
            return; // ���ص�ͼѭ��
        }
        else{
            client.base(input, commonBannedCommands, 0);
        }
    }
}

// ��ʼ��
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    mapNow = make_unique<Map>();
    mapNow->load(1);
}
// ��Ϸ����ѭ��
//void GameLoop::loop() {
//    initGame();
//    while (TRUE){
//        mapLoop();
//        //commandLoop
//    }
//
//}
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
    if (character.status.Speed > player.status.Speed){
        playerTurn = false;
        cout << "���˵����ƹ���, �����" << character.status.ATK - player.status.DEF << "���˺�" << endl;
        if(player.isDead()){
            // ��������
            return;
        }
        else{
            cout << "���ʣ��Ѫ��:" << player.status.HP << endl;
        }
    }
    int turn = 0; // �غ���
    while (true){
        turn += 1;
        cout << "�غ�:" << turn << endl;
        // �ж�buff�Ƿ����
        for (auto iter = player.buffs.begin(); iter != player.buffs.end() ; iter++) {
            // ֻ��Ϊ0�Ź���
            if ((*iter).duration != 0){
                (*iter).duration--;
            }
            else{
                player.buffs.erase(iter);
            }
        }
        // ǰ���ж�ʹ�غϿ�������
        // ��һغ�
        if (playerTurn){
            cout << "��ҵĻغ�:����ж�" << endl;
            //TODO::����command����

            // ��һ���ǵ����ж�
            playerTurn = false;
            enemyTurn = true;

            // ���ܵ���
            if (character.isDead()){
                cout << "�������" << character.nameCN << "�����ʤ��";
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
                break;
            }
            else{
                cout << character.nameCN << "ʣ��HP: " <<character.status.HP << endl;
            }
        }
        if (enemyTurn){
            int possible = getRandom(1, 100); // ��������
            int fluctuation = getRandom(90, 110); // �˺��Ĳ���
            int damage = int(character.status.ATK * fluctuation / 100);
            // ����
            if (possible >= character.status.Critical){
                damage = int(damage * 1.5);
            }
            player.status.HP -= (damage - player.status.DEF);
            if (player.isDead()){
                // ������������
                break;
            }
            else{
                cout << player.nameCN << "ʣ��HP: " << player.status.HP << endl;
            }
            // ��һ��������ж�
            playerTurn = true;
            enemyTurn = false;
        }
    }
}




