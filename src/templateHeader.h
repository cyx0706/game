//
// Created by cyx on 2019/9/1.
//

#ifndef GAME_TEMPLATEHEADER_H
#define GAME_TEMPLATEHEADER_H

#include <string>
#include <iostream>
#include <sstream>
#include "global.h"

template <class T>
class Package {
public:
    //属性
    explicit Package(int max = 25);
    Package(Package &aPackage)= default;
    vector<T> items;
    int maxItem; // 每个背包的最大大小
    //方法
    bool addItem(int itemId, int number);
    void showItems();
    void showItem(int itemId);
    bool deleteItem(int itemId, int number);
    bool deleteItem(int itemId);
};

/*
 * @brief 构造函数
 * 设置最大背包容量
 */
template <class T>
Package<T>::Package(int max) {
    this->maxItem = max;
}
/*
 * @brief 新加入一个物品
 *
 * @param itemId: 指定物品的id number: 指定数目
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
                return true;
            }
        }
        T item(itemId, number);
        this->items.push_back(item);
        return true;
    }
}
/*
 * @brief 删除整个物品
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
    cout << "未找到" << endl;
    return false;
}
/*
 * @brief 展示背包的物品
 */
template <class T>
void Package<T>::showItems() {
    for (auto iter = this->items.begin();  iter != this->items.end() ; iter++) {
        (*iter).showDescription();
    }
}
/*
 * @brief 获取指定id的物品的信息
 *
 * @param itemId:指定物品的id
 */
template <class T>
void Package<T>::showItem(int itemId) {
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
template <class T>
bool Package<T>::deleteItem(int itemId, int number) {
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
 * @param fpStream:文件流的引用 pack:类的模板引用
 * @return 文件流
 */
template <class T>
istream& operator>>(istream &fpStream, Package<T> &pack) {
    string temp;
    int itemId;
    int number;
    while (fpStream.peek() != EOF){
        fpStream >> temp >> itemId >> number;
        // 获取的类名为数字+类名, 好像这个还和编译器有关
        string className = typeid(T).name();
        className = className.substr(1);
        if (temp == className){
            T item(itemId, number);
            pack.items.push_back(item);
        }
    }
    return fpStream;
}
template <class T>
string toString(const T &v){
    ostringstream os; // 创建字符串输出流
    os << v; // 将变量写入字符串流
    return os.str(); // 返回输出流生成的字符串
}


template <class T>
T fromString(const string& str){
    if (str[0] == '-'){
        istringstream is(str.substr(1));
        int v;
        is >> v;
        return -v;
    }
    istringstream is(str); // 创建字符串输入流
    T v;
    is >> v; // 从字符串输入流中读取变量v
    return v;
}


#endif //GAME_TEMPLATEHEADER_H
