//
// Created by cyx on 2019/8/27.
//
#include "GameLoop.h"
/*
 * @brief 游戏的地图循环
 * 需要初始化完成后才能调用
 */
void GameLoop::mapLoop() {
    char input;
    while(TRUE)
    {
        if(kbhit())
        {
            mapNow->clean(uPos);//清除原有输出
            input = getch();
            // 输入Esc转换为命令行模式,返回true通知主调函数
            if (input == ESC){
                system("cls"); // 清空屏幕
                commandLoop();
                break;
            }
            if(input == SPACE){
                mapNow->checkEvent();
                mapNow->print();
            }
            else{
                mapNow->move(input);
                // 移动到下一章地图
                if (mapNow->checkTopMapTransition()||mapNow->checkBottomMapTransition()){
                    system("cls");
                    cout << "Waiting......";
                    system("cls");
                    // 生成指针并读取数据
                    mapNow = mapNow->nextMap(mapNow->roadTo[uPos]);
                    // 绘制地图
                    mapNow->initMap();
                    mapNow->checkSpecialScene();
                }
                mapNow->print();
            }
        }
    }
}

/*
 * @brief 命令行循环
 * 输入esc命令切换,在mapLoop里被调用
 */
void commandLoop(){
    string input;
    while (TRUE){
        int index = 0;
        getline(cin, input);
        string s = input;
        if (!s.empty()){
            // 循环删除所有的空格
            while ((index = s.find(' ', index)) != string::npos){
                s.erase(index); // 删除空格
            }
        }
        if (s == "esc"){
            return; // 返回地图循环
        }
        else{
            client.base(input);
        }


    }
}
// 初始化
void GameLoop::initGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}
// 游戏的主循环
//void GameLoop::loop() {
//    initGame();
//    while (TRUE){
//        mapLoop();
//        //commandLoop
//    }
//
//}
void GameLoop::gameStart() {

}