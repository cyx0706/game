cmake_minimum_required(VERSION 3.14)
project(game)

set(CMAKE_CXX_STANDARD 17)

add_executable(game main.cpp src/Character.h src/Package.h src/Item.h src/Map.h src/Mission.h src/Data.h src/Skill.h src/GameLoop.h src/Tool.cpp src/Tool.h src/Client.cpp src/Client.h src/Map.cpp src/GameLoop.cpp src/global.h src/Scene.cpp src/Scene.h src/Character.cpp src/Item.cpp src/Package.cpp src/Status.cpp src/Mission.cpp src/Skill.cpp src/templateHeader.h)