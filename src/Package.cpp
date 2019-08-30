//
// Created by cyx on 2019/8/30.
//
#include "Package.h"
#include "Item.h"
#include <iostream>
/*
 * @brief ���캯��
 * ������󱳰�����
 */
Package::Package() {
    this->maxItem = 50;
}
/*
 * @brief �¼���һ����Ʒ
 *
 * @param itemId: ָ����Ʒ��id number: ָ����Ŀ
 */
bool Package::addItem(int itemId, int number) {
    if (items.size() >= this->maxItem){
        return false;
    }
    else{
        for (auto iter = this->items.begin();  iter != this->items.end() ; iter++) {
            if ((*iter).id == itemId){
                (*iter).num += number;
            }
        }
        Item item(itemId, number);
        this->items.push_back(item);
        return true;
    }
}
/*
 * @brief չʾ��������Ʒ
 */
void Package::showItems() {
    for (auto iter = this->items.begin();  iter != this->items.end() ; iter++) {
        cout << "չʾ������������Ʒ" << endl;
        (*iter).showDescription();
    }
}
/*
 * @brief ��ȡָ��id����Ʒ����Ϣ
 *
 * @param itemId:ָ����Ʒ��id
 */
void Package::showItem(int itemId) {
    for (auto iter = this->items.begin(); iter != this->items.end(); iter++) {
        if ((*iter).id == itemId){
            cout << "չʾidΪ" << itemId << "����Ʒ��Ϣ" << endl;
            (*iter).showDescription();
            break;
        }
    }
    cout << "δ�ҵ�����Ʒ" << endl;
}
/*
 * @brief ɾ����Ʒ
 *
 * @param itemId: ָ����Ʒ��id number: ָ����Ŀ
 */
bool Package::deleteItem(int itemId, int number) {
    for (auto iter = this->items.begin(); iter != this->items.end(); iter++) {
        if((*iter).id == itemId){
            if ((*iter).num > number){
                (*iter).num -= number;
                return true;
            }
            if((*iter).num == number){
                this->items.erase(iter); // ɾ��
                return true;
            }
        }
    }
    return false;
}
/*
 * @brief ����>>���ڶ���Package��
 *
 * @param fpStream:�ļ��������� pack:�������
 * @return �ļ���
 */
istream& operator>>(istream &fpStream, Package &pack) {

}
/*
 * @brief ��ȡ�ļ���ʼ���̵�
 */
// TODO:���캯��
Shop::Shop() {

//    this->aPackage
}
/*
 * @brief ������Ʒ�Ľӿ�
 *
 * @param itemId: ��Ʒid, number: �������, money: ��ҽ�Ǯ������
 * @return �Ƿ���ɹ�
 */
bool Shop::buy(int itemId, int number, int &money) {
    for (auto iter = aPackage.items.begin();  iter!= aPackage.items.end() ; iter++) {
        if((*iter).id == itemId && (*iter).num >= number){
            int price = (*iter).boughtPrice * number;
            if (money >= price){
                money -= price;
                aPackage.deleteItem(itemId, number);
                cout << "��ӭ�´ι���" << endl;
                return true;
            }
            else{
                cout << "����" << endl;
                break;
            }
        }
    }
    cout << "û�и���Ʒ����Ʒ��Ŀ����" << endl;
    return false;
}

