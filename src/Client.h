//
// Created by Leo on 2019-08-26.
//

#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <string>
#include <map>
#include <vector>

using namespace std;

/*
 * @brief 所有的命令 并 对应一个数字
 */
enum CommandLists {
    attack = 1,
    skill,
    flee,
    purchase,
    sell,
    status,
    equipment,
    package,
    item,
    equip,
    takeoff,
    discard,
    talk,
    fight,
    slaughter,
    maps,
    help,
    mission,
    save,
};

/*
 * @brief 命令行类
 *
 * @note 调用
 * @code
 *
 * auto client = new Client();
 * client->base();
 *
 * @endcode
 */
class Client {
public:
    Client();
    void base(string str);
    auto findCommand(vector<string> commands);

private:
    map<string, string> commandsMap;
};


#endif //TEST_CLIENT_H
