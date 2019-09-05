//
// Created by cyx on 2019/8/27.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "global.h"
#include <iostream>

void dynamicScene1();
void dynamicScene2();
void dynamicScene3();
void dynamicScene4();
void dynamicScene5();

class Scene {
public:
    explicit Scene(int sceneId);
    int sceneId;
    string content;
    string path;
    static void show(string& sentence);
    void branch();
    void displayScene();
    friend istream& operator>>(istream &fpStream, Scene &scene);
};


#endif //GAME_SCENE_H
