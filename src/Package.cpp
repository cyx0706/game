//
// Created by cyx on 2019/8/30.
//
#include <fstream>
#include "Package.h"
#include "Item.h"
#include "Character.h"
#include "global.h"
#include "templateHeader.h"
extern Player player;

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


