//
// Created by lenovo on 2019/9/8.
//

#ifndef GAME_UI_H
#define GAME_UI_H

extern HANDLE hOut;

class UI {
public:
    static void pos(int x, int y);//确定光标位置
    //void blue_border();//蓝色字体
    static void white_back();//还原亮白色字体
    static void cyan_choose();//青色字体
    static void on_Select();//被选中时的样式
    static void onChoose(int x, int y);//确定所选中的位置
    static void star();//初始化界面
    static void setYellow();
    void setPurple();
    void setBlue();
    void blackOnWhite();
    static void redOnWhite();
    void redOnGrey();
    static void printTitle();
};

#endif //GAME_UI_H
