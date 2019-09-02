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

