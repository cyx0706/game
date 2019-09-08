//
// Created by lenovo on 2019/9/8.
//
#include<windows.h>//控制台编程主要头文件
#include<wincon.h>
#include "UI.h"
#include "global.h"


void UI::pos(int x, int y) {
    COORD posPoint = {static_cast<SHORT>(x),static_cast<SHORT>(y)}; //设置坐标
    SetConsoleCursorPosition(hOut,posPoint);
}

void UI::white_back() {
    WORD white=FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(hOut,white);
}

void UI::cyan_choose() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
                                                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void UI::on_Select() {
    redOnWhite();
}

void UI::onChoose(int x, int y) {
    if(x==47&&y==15)
    {
        on_Select();
        cout<<"  NewGame  ";
    }
    else if(x==47&&y==25)
    {
        on_Select();
        cout<<"    Out    ";
    }
    else if(x==47&&y==20)
    {
        on_Select();
        cout<<" Continue  ";
    }
}

void UI::star() {
    cyan_choose();
    pos(47,15);
    cout<<"  NewGame  ";
    pos(47,25);
    cout<<"    Out    ";
    pos(47,20);
    cout<<" Continue  ";
}

void UI::setYellow() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_GREEN);
}

void UI::setPurple() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_BLUE);
}

void UI::setBlue() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void UI::blackOnWhite() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}

void UI::redOnWhite() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
                                                            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
                                                            FOREGROUND_RED);
}

void UI::redOnGrey() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |BACKGROUND_INTENSITY |
                                                            FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void UI::printTitle() {
    setYellow();
    pos(42,3);
    cout<<"********************";
    pos(47,5);
    cout<<"摸鱼攻城狮"<<endl;

    pos(51,7);
    cout<<"の"<<endl;

    pos(46,9);
    cout<<"异世界狂想曲"<<endl;
    pos(42,11);
    cout<<"********************";
    for(int x = 42, y = 4; y <= 10;y ++){
        pos(x,y);
        cout<<"*";
    }
    for(int x = 61, y = 4; y <= 10;y ++){
        pos(x,y);
        cout<<"*";
    }
}
