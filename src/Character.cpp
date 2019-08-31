//
// Created by cyx on 2019/8/30.
//
#include "Character.h"

#include <utility>
#include "Item.h"
#include "Status.h"

// ----------------------Character类----------------------

/*
 * @brief 完整的构造函数
 *
 * @param status:状态
 *        id:标识
 *        nameCN:中文名
 *        nameEN:英文名
 *        fallingExp:掉落经验
 *        description:描述
 *        location:位置
 *        display:地图上的显示
 */
Character::Character(Status status,
                     string id,
                     string nameCN,
                     string nameEN,
                     string description,
                     Location location,
                     char display) {
    this->status = status;
    this->id = std::move(id);
    this->nameEN = std::move(nameEN);
    this->nameCN = std::move(nameCN);
    this->description = std::move(description);
    this->mapLocation = location;
    this->displayChar = display;
}

/*
 * @brief 简易的构造函数
 * 所有的初始化为空
 *
 */
Character::Character() {
    Status status1;
    this->status = status1;
    this->nameCN = "";
    this->nameEN = "";
    this->mapLocation = {1, 1, 1};
    this->description = "";
    this->displayChar = '\0';
}

/*
 * @brief 判断是否死亡
 *
 * @return 返回bool类型, true表示死亡
 */
bool Character::isDead() {
    return (this->status.HP < 0);
}

/*
 * @brief 展示信息
 *
 */
void Character::showDescription() {
    cout << this->description << endl;
}

// ---------------------Monster类-----------------------

Monster::Monster(string id) :Character(){
    // TODO:读取文件初始化
    this->id = std::move(id);
}


// --------------------Player类-------------------------

/*
 * @brief 构造函数
 */
Player::Player() :Character(){
    this->id = "PY-00"; // 固定的id
    this->experiencePoint = 0;
    this->days = 0;
    this->money = 0;
    this->Lv = 1;
    this->weapon = Weapon(0);
    this->armor = Armor(0);
    // 初始化一下背包
    this->weaponBag = Package<Weapon>();
    this->armorBag = Package<Armor>();
    this->drugBag = Package<Drug>();
    this->itemBag = Package<Item>();
}

/*
 * @brief 增加玩家的钱
 *
 * @param addition:增加数额
 */
void Player::addMoney(int addition) {
    if (money < 100000){
        money += addition;
    }
    else{
        cout << "金钱到达上限" << endl;
    }
}

/*
 * @brief 增加经验,顺便检查是否升级了
 *
 * @param addition:增加数额
 */
void Player::addExp(int addition) {
    if(Lv > 20){
        cout << "满级无法增加经验" << endl;
        return;
    }
    experiencePoint += addition;
    levelUp();
    cout << "当前等级" << Lv << endl;
}
/*
 * @brief 升级函数
 * 经验需求按线性增长
 * @return 是否升级
 */
void Player::levelUp() {
    int nextLvExp = 100 * (Lv + 1);
    while (experiencePoint > nextLvExp){
        experiencePoint -= nextLvExp;
        cout << nameCN << "升级了" << endl;
        Lv += 1;
        if (Lv == 3){
            // 学习新的技能
            cout << "学会了新的技能" << endl;
        }
        nextLvExp = 100 * (Lv + 1);
    }
    cout << "剩余 " << nextLvExp - experiencePoint << " 经验升到下一级" << endl;
}

// -----------------------NPC类-----------------------

NPC::NPC(string id):Character() {
    this->id = std::move(id);
    //TODO:读取文件初始化
}

/*
 * @brief 用户菜单
 */
void NPC::NPCMenu() {
    cout << this->nameCN << ":" << endl;
    if (this->bar){
        cout << "你可以在这里休息" << endl;
    }
    if (this->shopStatus){
        cout << "可以交易" << endl;
    }
    if (this->missionStatus){
        cout << "可以接任务" << endl;
    }
}

/*
 * @brief 和npc交易的函数,需要提前检查是否可以交易
 *
 * @param itemId:物品id number:购买个数 money:玩家的钱
 */
void NPC::buy(int itemId, int number, int &money) {
    if (!this->shopStatus){
        cout << "无法交易" << endl;
    }
    this->store.buy(itemId, number, money);
}

/*
 * @brief 卖出物品的函数
 *
 * @param itemId:物品id number:购买个数 money:玩家的钱
 */
void NPC::sell(Item &item, int number, int &money) {
    if (!this->shopStatus){
        cout << "无法交易" << endl;
    }
    this->store.sell(item, number, money);
}

/*
 * @brief 判断是否死亡
 * 如果死亡置为不可见
 */
bool NPC::isDead() {
    if (this->status.HP <= 0){
        this->isVisible = false;
        return true;
    }
    return false;
}

/*
 * @brief 多态的展示描述
 */
void NPC::showDescription() {
    cout << "NPC:" << this->nameCN << "(" << this->nameEN << ")";
}

/*
 * @brief 接受任务, 检查npc任务列表的第一个是否被接受
 *
 * @param player 玩家的引用
 */
void NPC::assignQuest(Player &player) {
    if (!missionStatus){
        cout << "无法发布任务" << endl;
        return;
    }
    for (unsigned int i = 0; i < questList.size(); i++) {
        //第一个未接受的任务
        if(!questList[i].isAccepted){
            if (!player.addMission(questList[i])){
                cout << "任务接受过了" << endl;
                return;
            }
            else{
                // 接任务时的对话
                cout << talkContent[questList[i].id][0] << endl;
                cout << "接受了任务" << endl;
                return;
            }
        }
    }
    cout << "没有任务可以接了" << endl;
}

/*
 * @brief 提交任务
 *
 * @param player:玩家的引用
 */
void NPC::finishQuest(Player &player) {
    if (!missionStatus){
        cout << "无法提交任务" << endl;
    }
    // 获取任务
    Mission* acceptedMission = player.getMission(this->id);
    // 判断是否获取成功
    if (acceptedMission == nullptr){
        cout << "没有要提交的任务" << endl;
    }
    else{
        // 检查是否可以提交
        // 若可以则要修改任务属性为完成
        // 由于引用同时修改了npc的任务的属性
        acceptedMission->checkFinished();
    }
}

/*
 * @brief 设置npc是否在地图上隐藏
 */
void NPC::setVisibility(bool isVisible) {
    this->isVisible = isVisible;
}

/*
 * @brief 获取npc是否隐藏
 * 地图函数需要调用
 */
bool NPC::getVisibility() {
    return this->isVisible;
}

/*
 * @brief 和NPC的对话
 * 不同任务的进行阶段的对话不同
 */
void NPC::talk(Player &player) {
    // 可以发任务的才有不同的对话
    if (missionStatus){
        Mission *mission = player.getMission(this->id);
        if (mission != nullptr){
            int id = mission->id;
            if(mission->isProcess){
                // 进行中时的对话
                cout << talkContent[id][1] << endl;
                return;
            }
        }
    }
    // 平时谈话
    cout << talkContent[0][0] << endl;
}

/*
 * @brief 用于检查是否无法对话直接进入战斗
 *
 * @param player:玩家的引用
 * @return 是否会发生战斗
 */
bool NPC::forceBattleCheck(Player &player) {
    // 拥有皇城通行证
    if (player.getItem(302) != 0){
        // 检查npc战斗状态
        if (this->battleStatus){
            cout << "没有什么好说的" << endl;
        }
    }
}