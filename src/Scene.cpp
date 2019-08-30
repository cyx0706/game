//
// Created by cyx on 2019/8/27.
//
#include "Scene.h"
#include "global.h"
#include "Character.h"
#include <windows.h>
#include <iostream>
extern Player player;
/*
 * @brief ���캯��,��ʼ����ͼ��id����branch���ж�
 */
Scene::Scene(int sceneId) {
    this->sceneId = sceneId;
}
/*
 * @brief չʾ����ľ���,��ʱ50s��ӡ
 *
 */
void Scene::show() {
    for (auto i = content.begin(); i != content.end() ; i++) {
        cout << *i;
        Sleep(50);
    }
}
/*
 * @brief ���ļ�������鲢չʾ����
 *
 * @param fpStream: ������������, scene:���������
 */
istream& operator>>(istream &fpStream, Scene &scene) {
    string temp;
    fpStream >> temp >> scene.sceneId;
    while ( temp != "[end]"){
        fpStream >> temp;
        if (temp == "[branch]"){
            scene.branch();
            continue;
        }
        if (temp == "[battle]"){
            //ս��
            continue;
        }
        fpStream >> scene.content;
        if (temp == "[template]"){
            int start = scene.content.find_first_of('<');
            int end = scene.content.find_first_of('>');
            // �滻ģ��Ϊ�û���
            scene.content.replace(start, end - start + 2 ,player.nameCN);
        }
        scene.show();
    }
    return fpStream;
}

void Scene::branch() {
    switch (this->sceneId){
        case 1: dynamicScene1();
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
        cout << "���ڽ����﷢����һ���Źַ�װ����\n"
                "�㳢�Դ򿪣�������˿������\n"
                "��һ������ħ����ӡ�˰ɣ��񵽱���\n";
        cout << "��ϵͳ��ʾ��:������ص��ռ�" << endl;
    }
    do{
        cout << "��ô����ô����: 1�Ѳ��������, 2ǰ��¥��" << endl;
        cin >> choose;
        if (choose == 1){
            cout << "��ʲôҲû����" << endl;
        }
    }while (choose != 2);
}

void dynamicScene3(){

}

void dynamicScene4(){

}
void dynamicScene5(){

}