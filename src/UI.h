//
// Created by lenovo on 2019/9/8.
//

#ifndef GAME_UI_H
#define GAME_UI_H

extern HANDLE hOut;

class UI {
public:
    static void pos(int x, int y);//ȷ�����λ��
    //void blue_border();//��ɫ����
    static void white_back();//��ԭ����ɫ����
    static void cyan_choose();//��ɫ����
    static void on_Select();//��ѡ��ʱ����ʽ
    static void onChoose(int x, int y);//ȷ����ѡ�е�λ��
    static void star();//��ʼ������
    static void setYellow();
    void setPurple();
    void setBlue();
    void blackOnWhite();
    static void redOnWhite();
    void redOnGrey();
    static void printTitle();
};

#endif //GAME_UI_H
