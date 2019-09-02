//
// Created by Leo on 2019-08-26.
//

#include "Client.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

#include "Tool.h"
#include "global.h"

/*
 * @brief �������Ҳ�����Ӧ����ʱ����� command not found
 */
void commandNotFound(){
    cout << "command not found" << endl;
}

/*
 * @brief ���ڵ�ǰ����������ֹʱ����� command is banned
 */
void commandIsBanned() {
    cout << "command is banned" << endl;
}

/*
 * @brief ���� ����������� map
 * ���ļ� commands.txt ��ȡ ���� �� ��ʾ��ֵ������commandsMap��
 * ���� �� ��ֵ �������Ϸ� ö������ CommandLists һ��
 *
 * commands.txt �ļ����� (���¸�ʽ):
 * * * * * * * * * * * * * * * * * * * * * * * *
 * -ս��ָ��
 * attack 1
 * / attack <character_name>
 * // ��ս��״̬��ʹ����ͨ���� ����ָ��Ŀ��
 *
 * ��'-'��ͷΪ   �������
 * ��'/ '��ͷΪ  �����÷�
 * ��'// '��ͷΪ ������
 *
 * ��ͷû�����η���Ϊ����
 * �������һ���� �ÿո����
 * * * * * * * * * * * * * * * * * * * * * * * *
 */
Client::Client() {
    // ��ִ���ļ������� build �ｫ command.txt ���ڸ�Ŀ¼/data��
    ifstream f(COMMANDS_TXT_PATH);

    string str;
    // һ�ζ�ȡһ��
    while(getline(f,str))
    {
        // ���С���ͷΪ'/'����ͷΪ'-'���� �޳�
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            // ȡ���� ���� �� ��ʾ��ֵ
            vector<string> keyValue = Tool::split(str);
            // ���� �� ��ʾ��ֵ ���� map ��
            this->commandsMap.insert(pair<string, string>(keyValue[0], keyValue[1]));
        }
    }

    f.close();
}

/*
 * @brief �������İ������
 *
 * @param command Ҫ�鿴����������
 */
void Client::commandHelp(string& command) {
    ifstream f(COMMANDS_TXT_PATH);

    string str;
    // һ�ζ�ȡһ�� ����Ҫ���ҵ������λ��
    while (getline(f,str)) {
        if (!str.empty() && str[0] != '/' && str[0] != '-') {
            str = Tool::split(str)[0];

            if (command == str) {
                break;
            }
        }
    }

    // �����ļ���β��Ϊû�д�������İ���
    if (f.eof()) {
        cout << "no command help" << endl;
    } else {
        while (getline(f,str)) {
            // �ļ��� ÿ������(����:��������÷���������)֮���ÿ��и���
            // �������������������а�����伴Ϊ������
            if (str.empty()) {
                break;
            }
            else {
                // ���ļ��д洢���������������ݽ��и�ʽ�滻
                // ��'/ ' ��ͷ��Ϊ�����÷�
                // ��'// '��ͷ��Ϊ������
                regex informationPattern("// ");
                regex usagePattern("/ ");

                // �����ȶ�'// '����ƥ�䣬��Ϊ'/ 'Ϊ'// '��һ����
                str = regex_replace(str, informationPattern, "���:");
                str = regex_replace(str, usagePattern, "�÷�:");

                cout << str << endl;
            }
        }
    }

    f.close();
}

/*
 * @brief ��������Ƿ񱻽�ֹ
 *
 * @param command ���������������(��ʾ��ֵ)
 * @param bannedCommands ����ֹ���õ�����
 * @return δ����ֹ�򷵻�false ����ֹ�򷵻�true
 */
bool Client::isBanned(CommandLists command, vector<int> &bannedCommands) {
    for (int one: bannedCommands) {
        if ((CommandLists)one == command) {
            return true;
        }
    }

    return false;
}

/*
 * @brief ��Ѱ��Ӧ���� ���ü�ִ��
 *
 * @param commands ���������������
 */
void Client::executeCommand(vector<string> commands,  vector<int>& bannedCommands, int index) {
    // Ѱ���Ƿ��� commandMap ���ж�Ӧ������
    // ���û�ж�Ӧ������ ִ�� commandNotFound()
    // commandMap.find() ���ص���������
    auto iter = commandsMap.find(commands[0]);
    if (iter != commandsMap.end()) {
        // �鿴�Ƿ��ǰ�������
        if (commands.size() == 2 && (commands[1] == "-h" || commands[1] == "--help")) {
            commandHelp(commands[0]);

            return;
        }

        // ������������������ʾʹ����ȷ������
        if (commands.size() == 2 && commands[1] == "-help") {
            cout << "use " << commands[0] << " -h or " << commands[0] << " --help" << endl;

            return;
        }

        // commandMap Ϊ map<string, string> ���ͣ���Ӧ ���� �� ��ʾ��ֵ
        // ��ʾ��ֵת��Ϊ int ����
        stringstream stream;
        int number = 0;
        string text(iter->second);
        stream << text;
        stream >> number ;
        stream.clear();

        auto command = (CommandLists)number;

        // ��������Ƿ����
        if (!isBanned(command, bannedCommands)) {
            // TODO
            switch (command) {
                case attack:
                    break;
                case skill:
                    break;
                case flee:
                    break;
                case purchase:
                    break;
                case sell:
                    break;
                case status:
                    break;
                case equipment:
                    break;
                case package:
                    break;
                case item:
                    break;
                case equip:
                    break;
                case takeoff:
                    break;
                case discard:
                    break;
                case talk:
                    break;
                case fight:
                    break;
                case slaughter:
                    break;
                case maps:
                    break;
                case help:
                    break;
                case mission:
                    break;
                case save:
                    exit(1);
                    break;
                default:
                    commandNotFound();
            }
        } else {
            commandIsBanned();
        }
    } else {
        commandNotFound();
    }
}

/*
 * @brief �����е� ��� ���� ����
 *
 * @param str �����һ������
 * @param bannedCommands ����ֹ������
 * @param index ������������Ӧ������λ��
 */
void Client::base(string str, vector<int>& bannedCommands, int index) {
        // ��ǰ��ո�ȥ���������ո��Ϊ�����ո�
        str = Tool::clean(str);

        if (!str.empty()) {
            vector commands = Tool::split(str, ' ');

            // �������������������Ӧ�ĺ���
            executeCommand(commands, bannedCommands, index);
        } else {
            cout << "empty command" << endl;
        }
}

