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
 * @brief 构造函数,初始化地图的id用于branch的判断
 */
Scene::Scene(int sceneId) {
    this->sceneId = sceneId;
    this->path = "../data/";
    this->path = path +"scene" + toString(sceneId) + ".txt";
}

/*
 * @brief 展示读入的剧情,延时50s打印
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
 * @brief 迭代文件实现每次读入一行的内容
 *
 * @param fpStream: 输入流的引用, scene:剧情的引用
 */
istream& operator>>(istream &fpStream, Scene &scene) {
    string temp;
    // 读标签
    fpStream >> temp;
    if ( temp != "[end]"){
        if (temp == "[branch]"){
            scene.branch();
            return fpStream;
        }
        // 读入内容
        fpStream >> scene.content;

        if (temp == "[template]"){
            int start = scene.content.find_first_of('<');
            int end = scene.content.find_first_of('>');
            // 替换模板为用户名
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
 * @brief 展示场景
 * 反复读文件,一次展示一句话
 * 相当于一个迭代器
 */
void Scene::displayScene() {
    sceneFp.open(path);
    string t;
    int sId;
    sceneFp >> t >> sId;
    while (sceneFp.peek()!=EOF){
        sceneFp >> *this;   // 读入并展示剧情
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
 * @brief 动态场景1,根据用户的输入或物品来分支剧情
 * 下面都相同,不再列出
 */
void dynamicScene1(){
    cout << "那么，怎么办呢: 1搜查这个屋子, 2前往楼下" << endl;
    int choose;
    cin >> choose;
    if (choose == 1){
        string t =  "你在角落里发现了一本古怪封装的书。你尝试打开，但书纹丝不动。";
        Scene::show(t);
        cout << "【系统提示】:获得神秘的日记" << endl;
        do{
            cout << "那么，怎么办呢: 1搜查这个屋子, 2前往楼下" << endl;
            cin >> choose;
            if (choose == 1){
                t = "你什么都没有发现";
                Scene::show(t);
                cout << endl;
            }
        }while (choose != 2);
    }
    // 清除缓存区
    cin.clear();
    cin.sync();
}

void dynamicScene2(){
    player.addItem(5);
    player.addItem(101);
    // 禁掉保存方法,场景中无法保存
    canSave = false;
    while (true){
        // 判断是否装备上了
        GameLoop::commandLoop();
        if(player.getWeapon()->id == 5 && player.getArmor()->id == 101){
            break;
        }
        else{
            cout << "你需要装备上全村最好的剑和全村最好的甲" << endl;
            system("pause");
        }
    }
    // 恢复保存
    canSave = true;
    GameLoop::battleLoop(globalNPC[1]); //和战士长打
    string t = "你合格了,我有一些任务要交给你办";
    Scene::show(t);
    t = "【系统提示】:你可以在战士长处接受任务";
    Scene::show(t);
    t = "【系统提示】:你获得了皇城的通行证";
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
            t = "前台：我可以帮你办理，但我们需要你的身份证明";
            Scene::show(t);
            t = "我：这么复杂的吗……，这个可以吗？";
            Scene::show(t);
            t = "你掏出来你的王城通行证";
            Scene::show(t);
            t = "埃伊娜：冒险者公会是无组织中立的，这个确实可以证明你的身份，但您现在是属于王国的直属，不能在我们这里登记，并且你没有办理费。";
            Scene::show(t);
            t = "你注意到埃伊娜的表情不像之前那么的明朗";
            Scene::show(t);
            cout << "该怎么办呢？" << endl;
            cout << "1(算了吧)\n"
                    "2我现在把这个和身上的装备卖掉\n";
            cin >> choose;
            if(choose == 1){
                cout << "你默默的离开了" << endl;
                system("pause");
                return;
            }
            else if(choose == 2) {
                t =  "埃伊娜的表情有些惊讶";
                Scene::show(t);
                t =  "埃伊娜：真是个有趣的人，可以，你稍等一下";
                Scene::show(t);
                t = "【系统提示】：失去王国通行证，获得冒险者纹章";
                Scene::show(t);
                t = "【系统提示】：无法再回到王国内城";
                Scene::show(t);
                t = "【系统提示】：获得100g";
                Scene::show(t);
                player.addMoney(100);
                player.addItem(301);
                // 防具去掉
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
                // 工会小姐姐可接任务
                globalNPC[3].missionStatus = true;
                system("pause");
                break;
            }
            else{
                cout << "我不能这么做" << endl;
                break;
            }
        }
        else if (choose == 2){
            return;
        }
        else{
            cout << "我不能这么做" << endl;
            cin.clear();
            cin.sync();
            continue;
        }
    }
    // 清除缓存区
    cin.clear();
    cin.sync();
}


void dynamicScene4(){

}
void dynamicScene5(){

}