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
 * @brief 读取文件初始化商店
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
 * @brief 购买物品的接口
 *
 * @param itemId: 物品id, number: 购买个数, money: 玩家金钱的引用
 * @return 是否购买成功
 */
bool Shop::buy(int itemId, int number, int &money) {
    int division = itemId / 100;
    int price = 0;
    //武器
    if (division == 0){
        for (auto iter = weaponPackage.items.begin();  iter != weaponPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (weaponPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "剩余个数不足" << endl;
                        return false;
                    }
                }
                else{
                    cout << "余额不足" << endl;
                    return false;
                }
            }
        }
    }
    // 防具
    else if (division == 1){
        for (auto iter = armorPackage.items.begin();  iter != armorPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (armorPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "剩余个数不足" << endl;
                        return false;
                    }
                }
                else{
                    cout << "余额不足" << endl;
                    return false;
                }
            }
        }
    }
    // 药物
    else if(division == 2){
        for (auto iter = drugPackage.items.begin();  iter != drugPackage.items.end() ; iter++) {
            if ((*iter).id == itemId){
                price = (*iter).boughtPrice * number;
                if (money >= price){
                    if (drugPackage.deleteItem(itemId, number)){
                        return true;
                    }
                    else{
                        cout << "剩余个数不足" << endl;
                        return false;
                    }
                }
                else{
                    cout << "余额不足" << endl;
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
 * @brief 卖出物品
 *
 * @param item:物品的引用 number:要卖出的个数 money:玩家钱的引用
 * @return 是否交易成功
 */
bool Shop::sell(Item &item, int number, int &money) {
    if (!item.canSell){
        cout << "无法卖出" << endl;
        return false;
    }
    if (item.num > number){
        item.num -= number;
    }
    else if(item.num == number){
        // 消除物品
        player.eraseItem(item.id);
    }
    else{
        cout << "物品数目不够" << endl;
        return false;
    }
    money += item.boughtPrice / 2 * number;
    return true;
}

/*
 * @brief 商店的菜单
 *
 */
void Shop::shopMenu() {
    cout << "武器" << endl
         << "英文名:名称:价格:数目" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = weaponPackage.items.begin();  iter!= weaponPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << (*iter).num << endl;

    }
    cout << "防具" << endl
         << "英文名:名称:价格:数目" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = armorPackage.items.begin();  iter!= armorPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << (*iter).num << endl;

    }
    cout << "药物" << endl
         << "英文名:名称:价格:数目" << endl;
    cout << "------------------------------" << endl;
    for (auto iter = drugPackage.items.begin();  iter!= drugPackage.items.end() ; iter++) {
        cout << (*iter).nameEN << " : "
             << (*iter).nameCN << " : "
             << (*iter).boughtPrice << " : "
             << "∞" << endl;

    }
    cout << "使用命令purchase和sell来进行交易" << endl;
}

/*
 * @brief 保存商店的物品
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


