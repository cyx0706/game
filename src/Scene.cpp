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
 * @brief 构造函数,初始化地图的id用于branch的判断
 */
Scene::Scene(int sceneId) {
    this->sceneId = sceneId;
}
/*
 * @brief 展示读入的剧情,延时50s打印
 *
 */
void Scene::show() {
    for (auto i = content.begin(); i != content.end() ; i++) {
        cout << *i;
        Sleep(50);
    }
}
/*
 * @brief 从文件读入剧情并展示出来
 *
 * @param fpStream: 输入流的引用, scene:剧情的引用
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
            //战斗
            continue;
        }
        fpStream >> scene.content;
        if (temp == "[template]"){
            int start = scene.content.find_first_of('<');
            int end = scene.content.find_first_of('>');
            // 替换模板为用户名
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
 * @brief 动态场景1,根据用户的输入或物品来分支剧情
 * 下面都相同,不再列出
 */
void dynamicScene1(){
    cout << "那么，怎么办呢: 1搜查这个屋子, 2前往楼下" << endl;
    int choose;
    cin >> choose;
    if (choose == 1){
        cout << "你在角落里发现了一本古怪封装的书\n"
                "你尝试打开，但书纹丝不动。\n"
                "这一定是用魔法封印了吧，捡到宝了\n";
        cout << "【系统提示】:获得神秘的日记" << endl;
    }
    do{
        cout << "那么，怎么办呢: 1搜查这个屋子, 2前往楼下" << endl;
        cin >> choose;
        if (choose == 1){
            cout << "你什么也没发现" << endl;
        }
    }while (choose != 2);
}

void dynamicScene3(){

}

void dynamicScene4(){

}
void dynamicScene5(){

}