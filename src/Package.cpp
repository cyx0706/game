//
// Created by cyx on 2019/8/30.
//
#include "Package.h"
#include "Item.h"
#include "Character.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include "global.h"
extern Player player;
#define SHOP_FILE_PATH "../data/map.txt"
/*
 * @brief ���캯��
 * ������󱳰�����
 */
template <class T>
Package<T>::Package(int max) {
    this->maxItem = max;
}
/*
 * @brief �¼���һ����Ʒ
 *
 * @param itemId: ָ����Ʒ��id number: ָ����Ŀ
 */
template <class T>
bool Package<T>::addItem(int itemId, int number) {
    if (items.size() >= this->maxItem){
        return false;
    }
    else{
        for (auto iter = this->items.begin();  iter != this->items.end() ; iter++) {
            if ((*iter).id == itemId){
                (*iter).num += number;
            }
        }
        T item(itemId, number);
        this->items.push_back(item);
        return true;
    }
}
/*
 * @brief ɾ��������Ʒ
 *
 * @param itemId:id
 */
template <class T>
bool Package<T>::deleteItem(int itemId) {
    for (auto iter = items.begin(); iter != items.end() ; iter++) {
        if ((*iter).id == itemId){
            items.erase(iter);
            return true;
        }
    }
    cout << "δ�ҵ�" << endl;
    return false;
}
/*
 * @brief չʾ��������Ʒ
 */
template <class T>
void Package<T>::showItems() {
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
template <class T>
void Package<T>::showItem(int itemId) {
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
template <class T>
bool Package<T>::deleteItem(int itemId, int number) {
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
 * @param fpStream:�ļ��������� pack:���ģ������
 * @return �ļ���
 */
template <class T>
istream& operator>>(istream &fpStream, Package<T> &pack) {
    string temp;
    int itemId;
    int number;
    while (fpStream.peek() != EOF){
        fpStream >> temp >> itemId >> number;
        if (temp == typeid(T).name()){
            T item(itemId, number);
            pack.items.push_back(item);
        }
    }
    return fpStream;
}

/*
 * @brief ��ȡ�ļ���ʼ���̵�
 */
Shop::Shop() {
    ifstream fp;
    fp.open(SHOP_FILE_PATH);
    fp >> this->weaponPackage;
    fp >> this->armorPackage;
    fp >> this->drugPackage;
    fp.close();
}

/*
 * @brief ������Ʒ�Ľӿ�
 *
 * @param itemId: ��Ʒid, number: �������, money: ��ҽ�Ǯ������
 * @return �Ƿ���ɹ�
 */
bool Shop::buy(int itemId, int number, int &money) {
    int division = itemId / 100;
    int price = 0;
    //����
    if (division == 0){
        for (auto iter = weaponPackage.items.begin();  iter != weaponPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (weaponPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "ʣ���������" << endl;
                        return false;
                    }
                }
                else{
                    cout << "����" << endl;
                    return false;
                }
            }
        }
    }
    // ����
    else if (division == 1){
        for (auto iter = armorPackage.items.begin();  iter != armorPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (armorPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "ʣ���������" << endl;
                        return false;
                    }
                }
                else{
                    cout << "����" << endl;
                    return false;
                }
            }
        }
    }
    // ҩ��
    else if(division == 2){
        for (auto iter = drugPackage.items.begin();  iter != drugPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (drugPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "ʣ���������" << endl;
                        return false;
                    }
                }
                else{
                    cout << "����" << endl;
                    return false;
                }
            }
        }
    }
    else{
        return false;
    }
    return false;
 }

/*
 * @brief ������Ʒ
 *
 * @param item:��Ʒ������ number:Ҫ�����ĸ��� money:���Ǯ������
 * @return �Ƿ��׳ɹ�
 */
bool Shop::sell(Item &item, int number, int &money) {
    if (!item.canSell){
        cout << "�޷�����" << endl;
        return false;
    }
    if (item.num > number){
        item.num -= number;
    }
    else if(item.num == number){
        // ������Ʒ
        player.eraseItem(item.id);
    }
    else{
        cout << "��Ʒ��Ŀ����" << endl;
        return false;
    }
    money += item.boughtPrice / 2 * number;
    return true;
}

/*
 * @brief �̵�Ĳ˵�
 *
 */
void Shop::shopMenu() {
    cout << "����" << endl
         << "Ӣ����:����:�۸�:��Ŀ" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = weaponPackage.items.begin();  iter!= weaponPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << (*iter).num << endl;

    }
    cout << "����" << endl
         << "Ӣ����:����:�۸�:��Ŀ" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = armorPackage.items.begin();  iter!= armorPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << (*iter).num << endl;

    }
    cout << "ҩ��" << endl
         << "Ӣ����:����:�۸�:��Ŀ" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = drugPackage.items.begin();  iter!= drugPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << "��" << endl;

    }
    cout << "ʹ������purchase��sell�����н���" << endl;
}

/*
 * @brief �����̵����Ʒ
 */
void Shop::save() {
    ofstream fp;
    fp.open(SHOP_FILE_PATH);
    for (auto iter = weaponPackage.items.begin();  iter!=weaponPackage.items.end() ; iter++) {
        fp << "Weapon"  << " "
           << (*iter).id << " "
           << (*iter).num << endl;
    }
    for (auto iter = armorPackage.items.begin(); iter != armorPackage.items.end() ; iter++) {
        fp << "Armor"  << " "
           << (*iter).id << " "
           << (*iter).num << endl;
    }
    for (auto iter = drugPackage.items.begin(); iter != drugPackage.items.end() ; iter++) {
        fp << "Drug"  << " "
           << (*iter).id << " "
           << (*iter).num << endl;
    }
    fp.close();
}


