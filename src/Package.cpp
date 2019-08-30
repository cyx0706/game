//
// Created by cyx on 2019/8/30.
//
#include "Package.h"
#include "Item.h"
#include <iostream>
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

}
/*
 * @brief 读取文件初始化商店
 */
// TODO:构造函数
Shop::Shop() {

//    this->aPackage
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

