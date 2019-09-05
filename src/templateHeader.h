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
    //����
    explicit Package(int max = 25);
    Package(Package &aPackage)= default;
    vector<T> items;
    int maxItem; // ÿ������������С
    //����
    bool addItem(int itemId, int number);
    void showItems();
    void showItem(int itemId);
    bool deleteItem(int itemId, int number);
    bool deleteItem(int itemId);
};

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
                return true;
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
        // ��ȡ������Ϊ����+����, ����������ͱ������й�
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
    ostringstream os; // �����ַ��������
    os << v; // ������д���ַ�����
    return os.str(); // ������������ɵ��ַ���
}


template <class T>
T fromString(const string& str){
    if (str[0] == '-'){
        istringstream is(str.substr(1));
        int v;
        is >> v;
        return -v;
    }
    istringstream is(str); // �����ַ���������
    T v;
    is >> v; // ���ַ����������ж�ȡ����v
    return v;
}


#endif //GAME_TEMPLATEHEADER_H
