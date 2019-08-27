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
