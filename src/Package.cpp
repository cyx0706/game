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
 * @brief 构造函数
 * 设置最大背包容量
 */
Package::Package() {
    this->maxItem = 50;
}
/*
 * @brief 新加入一个物品
 *
 * @param itemId: 指定物品的id number: 指定数目
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
 * @brief 展示背包的物品
 */
void Package::showItems() {
    for (auto iter = this->items.begin();  iter != this->items.end() ; iter++) {
        cout << "展示背包的所有物品" << endl;
        (*iter).showDescription();
    }
}
/*
 * @brief 获取指定id的物品的信息
 *
 * @param itemId:指定物品的id
 */
void Package::showItem(int itemId) {
    for (auto iter = this->items.begin(); iter != this->items.end(); iter++) {
        if ((*iter).id == itemId){
            cout << "展示id为" << itemId << "的物品信息" << endl;
            (*iter).showDescription();
            break;
        }
    }
    cout << "未找到该物品" << endl;
}
/*
 * @brief 删除物品
 *
 * @param itemId: 指定物品的id number: 指定数目
 */
bool Package::deleteItem(int itemId, int number) {
    for (auto iter = this->items.begin(); iter != this->items.end(); iter++) {
        if((*iter).id == itemId){
            if ((*iter).num > number){
                (*iter).num -= number;
                return true;
            }
            if((*iter).num == number){
                this->items.erase(iter); // 删除
                return true;
            }
        }
    }
    return false;
}
/*
 * @brief 重载>>便于读入Package类
 *
 * @param fpStream:文件流的引用 pack:类的引用
 * @return 文件流
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
 * @brief 读取文件初始化商店
 */
Shop::Shop() {
    ifstream fp;
    fp.open(SHOP_FILE_PATH);
    fp >> this->aPackage;
}

/*
 * @brief 购买物品的接口
 *
 * @param itemId: 物品id, number: 购买个数, money: 玩家金钱的引用
 * @return 是否购买成功
 */
bool Shop::buy(int itemId, int number, int &money) {
    for (auto iter = aPackage.items.begin();  iter!= aPackage.items.end() ; iter++) {
        if((*iter).id == itemId && (*iter).num >= number){
            int price = (*iter).boughtPrice * number;
            if (money >= price){
                money -= price;
                aPackage.deleteItem(itemId, number);
                cout << "欢迎下次光临" << endl;
                return true;
            }
            else{
                cout << "余额不足" << endl;
                break;
            }
        }
    }
    cout << "没有该物品或物品数目不够" << endl;
    return false;
}

/*
 * @brief 卖出物品
 *
 * @param item:物品的引用 number:要卖出的个数 money:玩家钱的引用
 * @return 是否交易成功
 */
bool Shop::sell(Item &item, int number, int &money) {
    if (item.canSell){
        if (item.num > number){
            item.num -= number;
        }
        else if(item.num == number){
            // 消除物品
            player.eraseItem(item);
        }
        else{
            cout << "物品数目不够" << endl;
            return false;
        }
        money += item.boughtPrice / 2 * number;
        return true;
    }
    else{
        cout << "物品不可被出售" << endl;
        return false;
    }
}
/*
 * @brief 商店的菜单
 *
 */
void Shop::shopMenu() {
    cout << "商品:价格:数目" << endl;
    for (auto iter = aPackage.items.begin();  iter!= aPackage.items.end() ; iter++) {
        // 不限制购买数目
        if ((*iter).num > 99){
            cout << (*iter).nameCN << ":" << (*iter).boughtPrice << ":" << "∞" << endl;
        }
        else{
            cout << (*iter).nameCN << ":" << (*iter).boughtPrice << ":" << (*iter).num << endl;
        }
    }
    cout << "使用命令purchase和sell来进行交易" << endl;
}

/*
 * @brief 保存商店的物品
 */
void Shop::save() {
    ofstream fp;
    fp.open(SHOP_FILE_PATH);
    for (auto iter = aPackage.items.begin();  iter!=aPackage.items.end() ; iter++) {
        fp << (*iter).id << (*iter).num << endl;
    }
}


