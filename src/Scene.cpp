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
        // TODO:调用战斗接口
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

void dynamicScene2(){
    // TODO:调用装备命令行
    // 判断是否装备上了
}

void dynamicScene3(){
    cout << "怎么办呢" << endl;
    int choose;
    while(true){
        cin >> choose;
        if (choose == 1){
            cout << "前台：我叫埃伊娜，你呢？\n"
                 << "我：我叫" << player.nameCN << endl
                 << "埃伊娜：你好有什么可以帮助你的吗\n"
                    "我：\n"
                    "2我想注册成为冒险者\n"
                    "3没什么\n"
                    "4有什么委托吗\n";
            continue;
        }
        else if(choose == 2){
            cout << "前台：我叫埃伊娜，我可以帮你办理，但我们需要你的身份证明\n"
                    "我：这么复杂的吗……，这个可以吗？\n"
                    "你掏出来你的王城通行证\n"
                    "埃伊娜：冒险者公会是无组织中立的，这个确实可以证明你的身份，但您现在是属于王国的直属，不能在我们这里登记，并且你没有办理费。\n"
                    "你注意到埃伊娜的表情不像之前那么的明朗\n"
                    "我：该怎么办呢？";
            cout << "1(算了吧)\n"
                    "2我现在把这个和身上的装备卖掉，用得到的钱注册可以吗";
            cin >> choose;
            if(choose == 1){
                break;
            }
            else if(choose == 2) {
                cout << "埃伊娜的表情有些惊讶\n"
                        "你一通讲自己的经历，并且表示很喜欢冒险者无忧的生活\n"
                        "埃伊娜：真是个有趣的人，可以，你稍等一下\n"
                        "【系统提示】：失去王国通行证，获得冒险者纹章\n"
                        "【系统提示】：无法再回到王国内城，但冒险者协会可以提供休息\n"
                        "【系统提示】：获得100g\n";
                player.addMoney(100);
                player.addItem(301);
                player.deleteItem(302);
                break;
            }
            else{
                cout << "我不能这么做" << endl;
                break;
            }
        }
        else if(choose == 3){
            break;
        }
        else if(choose == 4){
            cout << "没啥委托" << endl;
            break;
        }
        else{
            cout << "我不能这么做" << endl;
            continue;
        }
    }
}


void dynamicScene4(){
    // TODO:根据玩家的物品判断

}
void dynamicScene5(){

}