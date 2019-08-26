cmake_minimum_required(VERSION 3.14)
project(game)

set(CMAKE_CXX_STANDARD 17)

add_executable(game main.cpp src/Character.h src/Buff.h src/Package.h src/Player.h src/NPC.h src/Monster.h src/Shop.h src/Item.h src/MissionItem.h src/Weapon.h src/Drug.h src/Armor.h src/Map.h src/Mission.h src/Command.h src/Data.h src/Skill.h src/GameLoop.h)