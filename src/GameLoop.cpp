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
    client.base();
//    string input;
//    while (true){
//        int index = 0;
//        getline(cin, input);
//        string s = input;
//        if (!s.empty()){
//            // ѭ��ɾ�����еĿո�
//            while ((index = s.find(' ', index)) != string::npos){
//                s.erase(index); // ɾ���ո�
//            }
//        }
//        if (s == "esc"){
//            return; // ���ص�ͼѭ��
//        }
//        else{

//        }
//    }
}

// ��ʼ��
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    mapNow = make_unique<Map>();
    mapNow->load(1);
    Skill skill1("A01");
    player.skills.push_back(skill1);
    Skill skill2("B01");
    player.skills.push_back(skill2);
    Skill skill3("H01");
    player.skills.push_back(skill3);
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
    system("cls");

    int damage;
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOut, &cursorInfo);
    if (character.status.Speed > player.status.Speed){
        playerTurn = false;
        damage = character.status.ATK - player.status.DEF;
        if (damage <= 0){
            damage = 1;
        }
        cout << "���˵����ƹ���, �����" << damage << "���˺�" << endl;
        if(player.isDead()){
            player.deadScene();
            return;
        }
        else{
            cout << "���ʣ��Ѫ��:" << player.status.HP << endl;
        }
    }
    int turn = 0; // �غ���
    while (true){
        turn += 1;
        cout << "-------------------" << endl;
        cout << "�غ�:" << turn << endl;
        cout << "-------------------" << endl;
        cout << "��ʣ��HP:" << player.status.HP << " ʣ��MP:" << player.status.MP << endl;
        // �ж�buff�Ƿ����
        for (auto iter = player.buffs.begin(); iter != player.buffs.end() ; iter++) {
            // ֻ��Ϊ0�Ź���
            if ((*iter).duration != 0){
                (*iter).duration--;
                if (iter->duration == 0){
                    cout << "Buff " << iter->name << "��ʧ" << endl;
                }
                else{
                    cout << "Buff " << iter->name << "ʣ��" << iter->duration << "�غ�" << endl;
                }
            }
            else{
                player.buffs.erase(iter);
                player.deleteBuff((*iter));
            }
        }
        // ǰ���ж�ʹ�غϿ�������
        // ��һغ�
        if (playerTurn){
            cout << "��ҵĻغ�:����ж�" << endl;
            client.base(character);
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
                system("pause");
                system("cls");
                break;
            }
            else{
                cout << character.nameCN << "ʣ��HP: " <<character.status.HP << endl;
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




