//
// Created by cyx on 2019/8/30.
//
#include <fstream>
#include <iomanip>
#include "Package.h"
#include "Item.h"
#include "Character.h"
#include "global.h"
#include "templateHeader.h"
extern Player player;

/*
 * @brief 读取文件初始化商店
 * 物品比较少所以对单个文件读三次, 判断T类型来构造物品
 */
Shop::Shop() {
    ifstream fp;
    fp.open(SHOP_FILE_PATH);
    fp >> this->weaponPackage;
    fp.close();

    fp.open(SHOP_FILE_PATH);
    fp >> this->armorPackage;
    fp.close();

    fp.open(SHOP_FILE_PATH);
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
                        money -= price;
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
                        money -= price;
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
                        money -= price;
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
void Shop::shopMenu(SCOORD &pos) {
    pos.X+=2; // 右移一些
    Map::gotoxy(pos);
    cout << "\t\t\t商店";

    pos.Y++;
    Map::gotoxy(pos);
    cout << "\t\t\t武器";

    pos.Y++;
    Map::gotoxy(pos);
    cout << setiosflags(ios_base::left);
    cout << setw(20) << "中文名"
         << setw(20) << "英文名"
         << setw(4) << "价格"
         << setw(4) << "个数";

    pos.Y++;
    Map::gotoxy(pos);
    for (auto iter = weaponPackage.items.begin();  iter!= weaponPackage.items.end() ; iter++) {
        cout << setw(20) << (*iter).nameCN
             << setw(20) << (*iter).nameEN
             << setw(4) << (*iter).boughtPrice
             << setw(4) << (*iter).num;
        pos.Y++;
        Map::gotoxy(pos);
    }

    pos.Y++;
    Map::gotoxy(pos);
    cout << "\t\t\t防具";

    pos.Y++;
    Map::gotoxy(pos);
    cout << setw(20) << "中文名"
         << setw(20) << "英文名"
         << setw(4) << "价格"
         << setw(4) << "个数";

    pos.Y++;
    Map::gotoxy(pos);
    for (auto iter = armorPackage.items.begin();  iter!= armorPackage.items.end() ; iter++) {
        cout << setw(20) << (*iter).nameCN
             << setw(20) << (*iter).nameEN
             << setw(4) << (*iter).boughtPrice
             << setw(4) << (*iter).num;
        pos.Y++;
        Map::gotoxy(pos);
    }

    pos.Y++;
    Map::gotoxy(pos);
    cout << "\t\t\t药物";

    pos.Y++;
    Map::gotoxy(pos);
    cout << setw(20) << "中文名"
         << setw(20) << "英文名"
         << setw(4) << "价格"
         << setw(4) << "个数";

    pos.Y++;
    Map::gotoxy(pos);
    for (auto iter = drugPackage.items.begin();  iter!= drugPackage.items.end() ; iter++) {
        cout << setw(20) << (*iter).nameCN
             << setw(20) << (*iter).nameEN
             << setw(4) << (*iter).boughtPrice
             << setw(4) << "∞";
        pos.Y++;
        Map::gotoxy(pos);
    }
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


