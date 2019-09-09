//
// Created by cyx on 2019/8/27.
//
#include "Scene.h"

#include <windows.h>
#include <iostream>
#include <fstream>

#include "global.h"
#include "Character.h"
#include "GameLoop.h"

extern vector<NPC>globalNPC;
extern Player player;
extern ifstream sceneFp;
extern bool canSave;
/*
 * @brief ���캯��,��ʼ����ͼ��id����branch���ж�
 */
Scene::Scene(int sceneId) {
    this->sceneId = sceneId;
    this->path = "../data/";
    this->path = path +"scene" + toString(sceneId) + ".txt";
}

/*
 * @brief չʾ����ľ���,��ʱ50s��ӡ
 *
 */
void Scene::show(string& sentence) {
    for (auto i = sentence.begin(); i != sentence.end() ; i++) {
        cout << (*i);
        Sleep(25);
    }
    cout << endl;
}

/*
 * @brief �����ļ�ʵ��ÿ�ζ���һ�е�����
 *
 * @param fpStream: ������������, scene:���������
 */
istream& operator>>(istream &fpStream, Scene &scene) {
    string temp;
    // ����ǩ
    fpStream >> temp;
    if ( temp != "[end]"){
        if (temp == "[branch]"){
            scene.branch();
            return fpStream;
        }
        // ��������
        fpStream >> scene.content;

        if (temp == "[template]"){
            int start = scene.content.find_first_of('<');
            int end = scene.content.find_first_of('>');
            // �滻ģ��Ϊ�û���
            scene.content.replace(start, end - start + 1 ,player.nameCN);
        }
        Scene::show(scene.content);
    }
    else{
        fpStream.seekg(0,ios::end);
    }
    return fpStream;
}

/*
 * @brief չʾ����
 * �������ļ�,һ��չʾһ�仰
 * �൱��һ��������
 */
void Scene::displayScene() {
    sceneFp.open(path);
    string t;
    int sId;
    sceneFp >> t >> sId;
    while (sceneFp.peek()!=EOF){
        sceneFp >> *this;   // ���벢չʾ����
    }
    sceneFp.close();
}

void Scene::branch() {
    switch (this->sceneId){
        case 1: dynamicScene1();
            break;
        case 2: dynamicScene2();
            break;
        case 3: dynamicScene3();
            break;
        case 4: dynamicScene4();
            break;
        case 5: dynamicScene5();
            break;
        default: exit(1);
    }
}

/*
 * @brief ��̬����1,�����û����������Ʒ����֧����
 * ���涼��ͬ,�����г�
 */
void dynamicScene1(){
    cout << "��ô����ô����: 1�Ѳ��������, 2ǰ��¥��" << endl;
    int choose;
    cin >> choose;
    if (choose == 1){
        string t =  "���ڽ����﷢����һ���Źַ�װ���顣�㳢�Դ򿪣�������˿������";
        Scene::show(t);
        cout << "��ϵͳ��ʾ��:������ص��ռ�" << endl;
        do{
            cout << "��ô����ô����: 1�Ѳ��������, 2ǰ��¥��" << endl;
            cin >> choose;
            if (choose == 1){
                t = "��ʲô��û�з���";
                Scene::show(t);
                cout << endl;
            }
        }while (choose != 2);
    }
    // ���������
    cin.clear();
    cin.sync();
}

void dynamicScene2(){
    player.addItem(5);
    player.addItem(101);
    // �������淽��,�������޷�����
    canSave = false;
    while (true){
        // �ж��Ƿ�װ������
        GameLoop::commandLoop();
        if(player.getWeapon()->id == 5 && player.getArmor()->id == 101){
            break;
        }
        else{
            cout << "����Ҫװ����ȫ����õĽ���ȫ����õļ�" << endl;
            system("pause");
        }
    }
    // �ָ�����
    canSave = true;
    GameLoop::battleLoop(globalNPC[1]); //��սʿ����
    string t = "��ϸ���,����һЩ����Ҫ�������";
    Scene::show(t);
    t = "��ϵͳ��ʾ��:�������սʿ������������";
    Scene::show(t);
    t = "��ϵͳ��ʾ��:�����˻ʳǵ�ͨ��֤";
    Scene::show(t);
    player.addItem(302);
    system("pause");
    system("cls");
    globalNPC[1].setVisibility(true);
}

void dynamicScene3(){
    int choose;
    string t;
    while(true){
        cin >> choose;
        if(choose == 1){
            t = "ǰ̨���ҿ��԰��������������Ҫ������֤��";
            Scene::show(t);
            t = "�ң���ô���ӵ��𡭡������������";
            Scene::show(t);
            t = "���ͳ����������ͨ��֤";
            Scene::show(t);
            t = "�����ȣ�ð���߹���������֯�����ģ����ȷʵ����֤�������ݣ���������������������ֱ������������������Ǽǣ�������û�а���ѡ�";
            Scene::show(t);
            t = "��ע�⵽�����ȵı��鲻��֮ǰ��ô������";
            Scene::show(t);
            cout << "����ô���أ�" << endl;
            cout << "1(���˰�)\n"
                    "2�����ڰ���������ϵ�װ������\n";
            cin >> choose;
            if(choose == 1){
                cout << "��ĬĬ���뿪��" << endl;
                system("pause");
                return;
            }
            else if(choose == 2) {
                t =  "�����ȵı�����Щ����";
                Scene::show(t);
                t =  "�����ȣ����Ǹ���Ȥ���ˣ����ԣ����Ե�һ��";
                Scene::show(t);
                t = "��ϵͳ��ʾ����ʧȥ����ͨ��֤�����ð��������";
                Scene::show(t);
                t = "��ϵͳ��ʾ�����޷��ٻص������ڳ�";
                Scene::show(t);
                t = "��ϵͳ��ʾ�������100g";
                Scene::show(t);
                player.addMoney(100);
                player.addItem(301);
                // ����ȥ��
                string armorName = "best-armor";
                if(player.takeOffArmor(armorName)){
                    player.eraseItem(101);
                }
                else{
                    player.eraseItem(101);
                }
                string weaponName = "best-sword";
                if (player.takeOffWeapon(weaponName)){
                    player.eraseItem(5);
                }
                else{
                    player.eraseItem(5);
                }

                player.deleteItem(302);
                // ����С���ɽ�����
                globalNPC[3].missionStatus = true;
                system("pause");
                break;
            }
            else{
                cout << "�Ҳ�����ô��" << endl;
                break;
            }
        }
        else if (choose == 2){
            return;
        }
        else{
            cout << "�Ҳ�����ô��" << endl;
            cin.clear();
            cin.sync();
            continue;
        }
    }
    // ���������
    cin.clear();
    cin.sync();
}


void dynamicScene4(){

}
void dynamicScene5(){

}