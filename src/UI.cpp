//
// Created by lenovo on 2019/9/8.
//
#include<windows.h>//����̨�����Ҫͷ�ļ�
#include<wincon.h>
#include "UI.h"
#include "global.h"

//��������
void UI::pos(int x, int y) {
    COORD posPoint = {static_cast<SHORT>(x),static_cast<SHORT>(y)};
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

//����ť��ʽ��Ϊѡ��״̬
void UI::on_Select() {
    //redOnWhite();
    setRed();
}

//�жϹ��λ�ôӶ��ı䰴ť��ʽ
void UI::onChoose(int x, int y) {
    if(x==42&&y==15)
    {
        on_Select();
        pos(42,15);
        cout<<"��    NewGame    ��";
        pos(43,14);
        cout<<"��������������������������������";
        pos(43,16);
        cout<<"��������������������������������";
    }
    else if(x==42&&y==25)
    {
        on_Select();
        pos(43,24);
        cout<<"��������������������������������";
        pos(43,26);
        cout<<"��������������������������������";
        pos(42,25);
        cout<<"��      Out      ��";
    }
    else if(x==42&&y==20)
    {
        on_Select();
        pos(43,19);
        cout<<"��������������������������������";
        pos(43,21);
        cout<<"��������������������������������";
        pos(42,20);
        cout<<"��    Continue   ��";
    }
}

//�����ʼ��ť��ʽ
void UI::star() {
    cyan_choose();
    pos(42,15);
    cout<<"��    NewGame    ��";
    pos(43,14);
    cout<<"��������������������������������";
    pos(43,16);
    cout<<"��������������������������������";

    pos(43,24);
    cout<<"��������������������������������";
    pos(43,26);
    cout<<"��������������������������������";
    pos(42,25);
    cout<<"��      Out      ��";
    pos(43,19);
    cout<<"��������������������������������";
    pos(43,21);
    cout<<"��������������������������������";
    pos(42,20);
    cout<<"��    Continue   ��";
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

//�������
void UI::printTitle() {
    SetConsoleTextAttribute(hOut,14);
    pos(22,3);
    cout<<"************************************************************";
    pos(45,5);
    cout<<"�� �� �� �� ʨ"<<endl;

    pos(51,7);
    cout<<"��"<<endl;

    pos(44,9);
    cout<<"�� �� �� �� �� ��"<<endl;
    pos(22,11);
    cout<<"************************************************************";
    for(int x = 22, y = 4; y <= 10;y ++){
        pos(x,y);
        cout<<"*";
    }
    for(int x = 81, y = 4; y <= 10;y ++) {
        pos(x, y);
        cout << "*";
    }
}

void UI::setRed() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
}
