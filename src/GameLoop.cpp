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
#include <commctrl.h>
#include "Status.h"
#include "Character.h"
#include "global.h"
#include "Client.h"
#include "UI.h"

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
extern bool returnToMain;
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
        if (returnToMain){
            mapNow->clean(uPos);
            break;
        }
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
                    mapNow->print();
                    continue;
                }
                if (branch == 0){
                    string barrierType = "monster";
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
    system("mode con cols=100 lines=40");
    NPC::readLastLine = 0; // ��ʼ��
    player = Player();
    mapNow = make_unique<Map>();
    mapNow->load(1);
    returnToMain = false;

}


void GameLoop::gameInterface(){
    Map::setCursorStatus(false);
    system("mode con cols=100 lines=100");//��ʼ����������С
    UI::printTitle();
    UI::cyan_choose();
    UI::pos(47,25);
    cout<<"    Out    ";
    UI::pos(47,20);
    cout<<" Continue  ";
    UI::on_Select();
    UI::pos(47,15);
    cout<<"  NewGame  ";
    int x=47,y=15;

    int ch1=0;
    int ch2=0;
    if (ch1=getch())
    {
        ch2=getch();//��һ�ε���getch()������ֵ224
        while(ch2 != 13)
        {
            UI::star();
            switch (ch2)//�ڶ��ε���getch()
            {
                case 72: y = y - 5;break;
                case 80: y = y + 5;break;
                default:break;
            }
            if(x>=47)
            {
                x=47;
            }
            if(y>=30)
            {
                y=15;
            }
            if(x<=47)
            {
                x=47;
            }
            if(y<=10)
            {
                y=25;
            }
            UI::pos(x,y);
            UI::onChoose(x,y);
            ch2=getch();
        }
    }
    UI::pos(0,0);
    UI::white_back();//��ԭĬ������
    if(x==47&&y==15)
    {
        for (unsigned int i = 0; i < npcName.size(); i++) {
            NPC aNPC(npcName[i], INIT_NPC_PATH);
            globalNPC.push_back(aNPC);
        }
        newGame();
        GameLoop::mapLoop();
    }
    if(x==47&&y==25)
    {
        exit(0);
    }
    if(x==47&&y==20)
    {
        player.load();
        for (unsigned int i = 0; i < npcName.size(); i++) {
            NPC aNPC(npcName[i], SAVE_NPC_PATH);
            globalNPC.push_back(aNPC);
        }
        returnToMain = false;
        uPos.X = short(player.mapLocation.x);
        uPos.Y = short(player.mapLocation.y);
        mapNow->initPos = uPos;
        mapNow->load(player.mapLocation.mapId);
        mapNow->print();
        GameLoop::mapLoop();
    }
}

void GameLoop::newGame() {
    Map::setCursorStatus(true);
    system("cls");
    cout << "��������û���" << endl;
    string input;
    cin >> input;
    transform(input.begin(), input.end(), input.begin(), ::tolower); // ת��Сд
    // ɾ����β�ո�
    if( !input.empty() ){
        input.erase(0,input.find_first_not_of(' '));
        input.erase(input.find_last_not_of(' ') + 1);
    }
    player.nameCN = input;
    player.nameEN = "None";
    player.maxHP = 150;
    Skill skill1("H01");
    player.skills.push_back(skill1);
    Skill skill2("A01");
    player.skills.push_back(skill2);
    Scene scene1(1);
    scene1.displayScene();
    Scene scene2(2);
    scene2.displayScene();
    dynamicScene2();
}


void GameLoop::npcLoop(NPC &talkedNPC) {
    system("cls");
    Map::setCursorStatus(true);
    if (!talkedNPC.NPCMenu(player)){
        client.npcBase(talkedNPC);
    }
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

    cout << "��" << character.nameCN << "��ս��" << endl;
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
        cout << "-> HP:" << player.status.HP << "/" << player.maxHP;
        curPos.Y++;
        Map::gotoxy(curPos);
        cout << "-> MP:" << player.status.MP << "/" << player.maxMP;
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
                    // �����˾���
                    if (monster->id == "NY-04"){
                        system("pause");
                        system("cls");
                        Scene scene(6);
                        scene.displayScene();
                        returnToMain = true;
                        system("pause");
                        system("cls");
                        break;
                    }
                    for(auto iter = monster->fallingItem.begin(); iter!=monster->fallingItem.end(); iter++){
                       cout << "���������" << (*iter).nameCN << endl;
                       player.addItem((*iter).id, (*iter).num);
                   }
                }
                // ���ܴ��˾
                else{
                    if (character.id == "NY-01"){
                        system("pause");
                        system("cls");
                        Scene s(7);
                        s.displayScene();
                        returnToMain = true;
                        system("pause");
                        system("cls");
                        break;
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
            if (possible <= character.status.Critical){
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


/*
 * @brief �̵�ѭ��, ������ҹ�����Ʒ���̵�ҳ��
 */
void GameLoop::shopLoop(NPC &npc) {
    Map::setCursorStatus(false);
    system("cls");
    Map::setCursorStatus(true);
    npc.shopMenu(player);
    client.shopBase(npc);
    system("cls");
}


