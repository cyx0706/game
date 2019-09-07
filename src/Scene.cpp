//
// Created by cyx on 2019/8/27.
//
#include "Scene.h"
#include "global.h"
#include "Character.h"
#include "Client.h"
#include "GameLoop.h"
#include <windows.h>
#include <iostream>
#include <fstream>
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
    fpStream >> temp;
    if ( temp != "[end]"){
        fpStream >> scene.content;
        if (temp == "[branch]"){
            scene.branch();
            return fpStream;
        }

        if (temp == "[template]"){
            int start = scene.content.find_first_of('<');
            int end = scene.content.find_first_of('>');
            // �滻ģ��Ϊ�û���
            scene.content.replace(start, end - start + 1 ,player.nameCN);
        }
        Scene::show(scene.content);
    }
    return fpStream;
}

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
    cout << "��ô����" << endl;
    int choose;
    while(true){
        cin >> choose;
        if (choose == 1){
            cout << "ǰ̨���ҽа����ȣ����أ�\n"
                 << "�ң��ҽ�" << player.nameCN << endl
                 << "�����ȣ������ʲô���԰��������\n"
                    "�ң�\n"
                    "2����ע���Ϊð����\n"
                    "3ûʲô\n"
                    "4��ʲôί����\n";
            continue;
        }
        else if(choose == 2){
            cout << "ǰ̨���ҽа����ȣ��ҿ��԰��������������Ҫ������֤��\n"
                    "�ң���ô���ӵ��𡭡������������\n"
                    "���ͳ����������ͨ��֤\n"
                    "�����ȣ�ð���߹���������֯�����ģ����ȷʵ����֤�������ݣ���������������������ֱ������������������Ǽǣ�������û�а���ѡ�\n"
                    "��ע�⵽�����ȵı��鲻��֮ǰ��ô������\n"
                    "�ң�����ô���أ�";
            cout << "1(���˰�)\n"
                    "2�����ڰ���������ϵ�װ���������õõ���Ǯע�������";
            cin >> choose;
            if(choose == 1){
                break;
            }
            else if(choose == 2) {
                cout << "�����ȵı�����Щ����\n"
                        "��һͨ���Լ��ľ��������ұ�ʾ��ϲ��ð�������ǵ�����\n"
                        "�����ȣ����Ǹ���Ȥ���ˣ����ԣ����Ե�һ��\n"
                        "��ϵͳ��ʾ����ʧȥ����ͨ��֤�����ð��������\n"
                        "��ϵͳ��ʾ�����޷��ٻص������ڳǣ���ð����Э������ṩ��Ϣ\n"
                        "��ϵͳ��ʾ�������100g\n";
                player.addMoney(100);
                player.addItem(301);
                player.deleteItem(302);
                break;
            }
            else{
                cout << "�Ҳ�����ô��" << endl;
                break;
            }
        }
        else if(choose == 3){
            break;
        }
        else if(choose == 4){
            cout << "ûɶί��" << endl;
            break;
        }
        else{
            cout << "�Ҳ�����ô��" << endl;
            continue;
        }
    }
}


void dynamicScene4(){
    // TODO:������ҵ���Ʒ�ж�

}
void dynamicScene5(){

}