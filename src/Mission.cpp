//
// Created by Leo on 2019-08-31.
//

#include "Mission.h"

#include <fstream>
#include <map>
#include "Tool.h"
#include "Character.h"
#include "global.h"
#include "templateHeader.h"

extern Player player;

/*
 * @brief ���ļ��ж�ȡ Mission ��Ļ�����Ϣ ������Ӧ�Ķ���
 *
 * @param id Mission �����Ӧ�� id
 *
 * mission.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * id
 * nameEN
 * nameCN
 * bonusMoney
 * bonusExperiencePoint
 * assigner
 * __ __
 * __ __
 * __ __
 *
 * __ __ ���� ��������������Ʒ
 * ��һ�� __ Ϊ ��Ӧ��Ʒ�� id
 * �ڶ��� __ Ϊ ��Ӧ��ƷҪ�������
 *
 * �ֱ��Ӧ Mission ��ĸ�������
 * ���м�ֵ�Ա������
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * ����
 * isAccepted
 * isFinished
 * isProcess
 * ��Ҫ�Ӵ浵�ж�ȡ
 */
Mission::Mission(int id) {
    ifstream f(MISSION_TXT_PATH);
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
    this->bonusMoney = fromString<int>(data["bonusMoney"]);
    this->bonusExperiencePoint = fromString<int>(data["bonusExperiencePoint"]);
    this->assigner = data["assigner"];

    f.close();

    // TODO �Ӵ浵�ж���
    this->isAccepted = true;
    this->isFinished = false;
    this->isProcess = false;

    // ��ȡ��������������Ʒ ��Ʒid �� ��Ʒ���� �ļ�ֵ�ԣ����� requiredItem
    for (const auto& one : data) {
        // �޳� map �������ļ�ֵ�� id nameCN nameEN bonusMoney bonusExperiencePoint assigner
        if (one.first == "id" ||
        one.first == "nameCN" || one.first == "nameEN" ||
        one.first == "bonusMoney" || one.first == "bonusExperiencePoint" ||
        one.first == "assigner") {
            continue;
        } else {
            this->requiredItem.insert(pair(fromString<int>(one.first), fromString<int>(one.second)));
        }
    }
}

/*
 * @brief ����Ƿ����������
 * �����ұ�������Ʒ �� ���������Ҫ����Ʒ ���жԱ�
 * �� δ�ﵽ����������� ���"����δ���" �����κδ���
 * �� �Ѵﵽ����������� ���"�������  "  ����ұ����м�ȥ ����������Ʒ����Ӧ����
 *                                    ��������� ���� �� ��Ǯ
 */
void Mission::checkFinished() {
    // ���� �����������Ӧ��Ʒ �Ƚ� ��ұ��������е�����
    for (const auto& one : this->requiredItem) {
        // ��� ��ұ������Ӧ��Ʒ������������������� ���"����δ���" return
        if (player.getItem(one.first) < one.second) {
            cout << "����δ���" << endl;
            return;
        }
    }

    // ��ȥ��ұ�������Ӧ����Ʒ����
    for (const auto& one : this->requiredItem) {
        player.deleteItem(one.first, one.second);
    }

    // �������
    cout << "�������" << endl;
    this->isFinished = true;
    // ��� ���ӽ�Ǯ �� ����
    player.addMoney(this->bonusMoney);
    player.addExp(this->bonusExperiencePoint);
}

void Mission::saveMission(string owner, string path) {
    ofstream of;
    of.open(path);
    map<string,string> m_map;
    //����player�ĵ�������
    m_map["owner"] = owner;
    m_map["id"] = toString<int>(id);
    m_map["nameEN"] = nameEN;
    m_map["nameCN"] = nameCN;
    m_map["isAccepted"] = toString<bool>(isAccepted);
    m_map["isFinished"] = toString<bool>(isFinished);
    m_map["isProcess"] = toString<bool>(isProcess);
    m_map["bonusMoney"] = toString<int>(bonusMoney);
    m_map["bonusExperiencePoint"] = toString<int>(bonusExperiencePoint);
    m_map["assigner"] = assigner;

    auto iter = m_map.begin();
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    of << "type requiredItem" << endl;
    auto iterator = requiredItem.begin();
    for(;iterator != requiredItem.end(); iterator ++){
        of << iterator->first << " " << iterator->second << endl;
    }
    m_map.clear();
    of << endl;
    of.close();
}

void Mission::loadMission(ifstream& f,string owner, string path) {
    //ifstream f(path);
    string str;

    // �ҵ���Ӧ owner ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "owner" && idLine[1] == owner) {
                break;
            }
        }
    }
    // ��ȡΪ��ֵ��
    map<string, string> data;

    while (getline(f, str)) {
        // �������н���
        if (!str.empty() || str != "requiredItem") {
            vector<string> keyValue = Tool::split(str);

            data.insert(pair<string, string>(keyValue[0], keyValue[1]));
        } else {
            break;
        }
    }

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->isProcess = Tool::boolFromString(data["isProcess"]);
    this->isFinished = Tool::boolFromString(data["isFinished"]);
    this->isAccepted = Tool::boolFromString(data["isAccepted"]);
    this->bonusMoney = fromString<int>(data["bonusMoney"]);
    this->bonusExperiencePoint = fromString<int>(data["bonusExperiencePoint"]);
    this->assigner = data["assigner"];

    data.clear();

    // �ҵ���Ӧ type ��
    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "requiredItem") {
                break;
            }
        }
    }
    while (getline(f, str)) {
        // �������н���
        if (!str.empty()) {
            vector<string> keyValue = Tool::split(str);

            requiredItem.insert(pair<int, int>(fromString<int>(keyValue[0]), fromString<int>(keyValue[1])));
        } else {
            break;
        }
    }

    f.close();
}

