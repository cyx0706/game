//
// Created by Leo on 2019-08-30.
//

#include "Item.h"

#include <map>
#include <fstream>

#include "Status.h"
#include "Tool.h"
#include "global.h"
#include "templateHeader.h"
/*
 * @brief �޲εĹ��캯�� ������Ʒ����ռλ ��Ϊ��
 */
Item::Item() {
    this->id = 0;
    this->nameCN = "";
    this->nameEN = "";
    this->description = "";
    this->num = 0;
    this->boughtPrice = 0;
    this->canDrop = false;
    this->canSell = false;
}

/*
 * @brief ���ļ��ж�ȡ�� Item ��Ļ�����Ϣ ������Ӧ�Ķ���
 *
 * @param id Item �����Ӧ�ı�ʾ id
 * @param number ������Ʒ������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * �ļ�����Ʒ�� id ��Ӧ:
 * 001 -- 099: ���� Weapon
 * 101 -- 199: ���� Armor
 * 201 -- 299: ҩ�� Drug
 * 301 -- 399: �������
 *
 * items.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id 1
 * nameCN ��
 * nameEN sword
 * description һ����ͨ�Ľ�������������������
 * boughtPrice 10
 * canDrop true
 * canSell true
 *
 * �ֱ��Ӧ Item ��ĸ�������
 * ���м�ֵ�Ա������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Item::Item(int id, int number) {
    ifstream f(ITEMS_TXT_PATH);
    string str;

    // �ҵ���Ӧ id ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // �� ��Ӧ id �е���һ������֮������ݶ�ȡΪ��ֵ��
    map<string, string> data = Tool::dataMap(f);

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->description = data["description"];
    this->num = number;
    this->boughtPrice = fromString<int>(data["boughtPrice"]);
    this->canDrop = Tool::boolFromString(data["canDrop"]);
    this->canSell = Tool::boolFromString(data["canSell"]);

    f.close();
}

/*
 * @brief ��ӡ��Ʒ�Ľ���
 */
void Item::showDescription() {
    // �޳� ռλ Item �������Ϊ��
    if (id != 0) {
        cout << "������:\t" << this->nameCN << endl;
        cout << "Ӣ����:\t" << this->nameEN << endl;
        cout << "���:\t" << this->description << endl;
        cout << "����:\t" << this->num << endl;
        cout << "��ֵ:\t" << this->boughtPrice << endl;

        if (this->canDrop) {
            cout << "����:\t" << "�ɶ���  ";
        } else {
            cout << "����:\t" << "���ɶ���";
        }

        if (this->canSell) {
            cout << "�ɽ���  " << endl;
        } else {
            cout << "���ɽ���" << endl;
        }

    } else {
        cout << "��" << endl;
    }
}

/*
 * @brief �޲εĹ��캯�� ��Ϊ��
 */
Drug::Drug() : Item(){
    this->HP = 0;
    this->MP = 0;
    this->Phy = 0;
    this->playerTarget = false;
}

/*
 * @brief ���ļ��ж�ȡ�� Drug ��Ļ�����Ϣ ������Ӧ�Ķ���
 *
 * @param id Item �����Ӧ�� id (id ��Χ: 201 -- 299)
 * @param number ������Ʒ������
 *
 * drug.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * buff
 * HP
 * MP
 * Phy
 * playerTarget
 *
 * �ֱ��Ӧ Drug ��ĸ�������
 * ���м�ֵ�Ա������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Drug::Drug(int id, int number) : Item(id, number){
    ifstream f(DRUG_TXT_PATH);
    string str;

    // �ҵ���Ӧ id ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // �� ��Ӧ id �е���һ������֮������ݶ�ȡΪ��ֵ��
    map<string, string> data = Tool::dataMap(f);

    this->HP = fromString<int>(data["HP"]);
    this->MP = fromString<int>(data["MP"]);
    this->Phy = fromString<int>(data["Phy"]);
    this->playerTarget = Tool::boolFromString(data["playerTarget"]);

    f.close();
}
/*
 * @brief չʾҩƷ����Ϣ
 */
void Drug::showDescription(){
    Item::showDescription();
    cout << "�ظ�Ѫ��:\t" << this->HP << endl;
    cout << "�ظ�����:\t" << this->MP << endl;
    cout << "�ظ�����ֵ:\t" <<  this->Phy << endl;
    if (this->playerTarget) {
        cout << "ʹ�÷�ʽ:\t" << "�Լ�" << endl;
    } else {
        cout << "ʹ�÷�ʽ:\t" << "�Ե�" << endl;
    }
}

/*
 * @brief ������Ĭ�Ϲ��캯��
 * ����һ���յ�����,idΪ0
 */
Weapon::Weapon() : Item() {
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
}

/*
 * @brief ���ļ��ж�ȡ�� Weapon ��Ļ�����Ϣ ������Ӧ�Ķ���
 *
 * @param id Item �����Ӧ�� id (id ��Χ: 001 -- 099)
 * @param number ������Ʒ������
 *
 * weapon.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * ATK
 * DEF
 * Speed
 *
 * �ֱ��Ӧ Weapon ��ĸ�������
 * ���м�ֵ�Ա������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Weapon::Weapon(int id, int number) : Item(id, number) {
    ifstream f(WEAPON_TXT_PATH);
    string str;

    // �ҵ���Ӧ id ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // �� ��Ӧ id �е���һ������֮������ݶ�ȡΪ��ֵ��
    map<string, string> data = Tool::dataMap(f);

    this->ATK = fromString<int>(data["ATK"]);
    this->DEF = fromString<int>(data["DEF"]);
    this->Speed = fromString<int>(data["Speed"]);

    f.close();
}

/*
 * @brief չʾ��������Ϣ
 */
void Weapon::showDescription() {
    Item::showDescription();
    cout << "����:" << this->ATK << endl;
    cout << "����:" << this->DEF << endl;
    cout << "�ٶ�:" << this->Speed << endl;
}

/*
 * @brief �޲εĹ��캯�� ��Ϊ��
 */
Armor::Armor() : Item(){
    this->ATK = 0;
    this->DEF = 0;
    this->Speed = 0;
}

/*
 * @brief ���ļ��ж�ȡ�� Armor ��Ļ�����Ϣ ������Ӧ�Ķ���
 *
 * @param id Item �����Ӧ�� id (id ��Χ: 101 -- 199)
 * @param number ������Ʒ������
 *
 * armor.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * ATK
 * DEF
 * Speed
 *
 * �ֱ��Ӧ Armor ��ĸ�������
 * ���м�ֵ�Ա������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */
Armor::Armor(int id, int number) : Item(id, number) {
    ifstream f(ARMOR_TXT_PATH);
    string str;

    // �ҵ���Ӧ id ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && fromString<int>(idLine[1]) == id) {
                break;
            }
        }
    }
    // �� ��Ӧ id �е���һ������֮������ݶ�ȡΪ��ֵ��
    map<string, string> data = Tool::dataMap(f);

    this->ATK = fromString<int>(data["ATK"]);
    this->DEF = fromString<int>(data["DEF"]);
    this->Speed = fromString<int>(data["Speed"]);

    f.close();
}

/*
 * չʾ���ߵ�������Ϣ
 */
void Armor::showDescription() {
    Item::showDescription();
    cout << "����:" << this->ATK << endl;
    cout << "����:" << this->DEF << endl;
    cout << "�ٶ�:" << this->Speed << endl;
}
