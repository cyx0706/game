//
// Created by cyx on 2019/8/30.
//
#include "Package.h"
#include "Item.h"
#include <iostream>
#include <fstream>
#include <ostream>
extern Player player;
#define SHOP_FILE_PATH "../data/map.txt"
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
    string temp;
    int itemId;
    int number;
    while (fpStream.peek() != EOF){
        fpStream >> temp >> itemId >> number;
        Item item(itemId, number);
        pack.items.push_back(item);
    }
    return fpStream;
}

/*
 * @brief ��ȡ�ļ���ʼ���̵�
 */
Shop::Shop() {
    ifstream fp;
    fp.open(SHOP_FILE_PATH);
    fp >> this->aPackage;
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

/*
 * @brief ������Ʒ
 *
 * @param item:��Ʒ������ number:Ҫ�����ĸ��� money:���Ǯ������
 * @return �Ƿ��׳ɹ�
 */
bool Shop::sell(Item &item, int number, int &money) {
    if (item.canSell){
        if (item.num > number){
            item.num -= number;
        }
        else if(item.num == number){
            // ������Ʒ
            player.eraseItem(item);
        }
        else{
            cout << "��Ʒ��Ŀ����" << endl;
            return false;
        }
        money += item.boughtPrice / 2 * number;
        return true;
    }
    else{
        cout << "��Ʒ���ɱ�����" << endl;
        return false;
    }
}
/*
 * @brief �̵�Ĳ˵�
 *
 */
void Shop::shopMenu() {
    cout << "��Ʒ:�۸�:��Ŀ" << endl;
    for (auto iter = aPackage.items.begin();  iter!= aPackage.items.end() ; iter++) {
        // �����ƹ�����Ŀ
        if ((*iter).num > 99){
            cout << (*iter).nameCN << ":" << (*iter).boughtPrice << ":" << "��" << endl;
        }
        else{
            cout << (*iter).nameCN << ":" << (*iter).boughtPrice << ":" << (*iter).num << endl;
        }
    }
    cout << "ʹ������purchase��sell�����н���" << endl;
}

/*
 * @brief �����̵����Ʒ
 */
void Shop::save() {
    ofstream fp;
    fp.open(SHOP_FILE_PATH);
    for (auto iter = aPackage.items.begin();  iter!=aPackage.items.end() ; iter++) {
        fp << (*iter).id << (*iter).num << endl;
    }
}


