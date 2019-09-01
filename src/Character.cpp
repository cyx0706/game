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
                     char display,
                     int fallingExp,
                     int fallingMoney) {
    this->status = status;
    this->id = std::move(id);
    this->nameEN = std::move(nameEN);
    this->nameCN = std::move(nameCN);
    this->description = std::move(description);
    this->mapLocation = location;
    this->displayChar = display;
    this->fallingExp = fallingExp;
    this->fallingMoney = fallingMoney;
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
    this->fallingMoney = 0;
    this->fallingExp = 0;
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
    this->weapon = Weapon();
    this->armor = Armor();
    // 初始化一下背包
//    this->weaponBag = Package<Weapon>();
//    this->armorBag = Package<Armor>();
//    this->drugBag = Package<Drug>();
//    this->itemBag = Package<Item>();
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
/*
 * @brief 装备防具
 * 卸下防具就是装备空防具
 * @param 命令行传入的字符串name
 * 当name=None时表示卸下防具
 */
bool Player::equipArmor(string &name) {
    Armor oldArmor = this->armor;
    // 卸下装备
    if( name == "None"){
        if (oldArmor.id == 0){
            cout << "当前没有装备" << endl;
            return false;
        }
        armorBag.addItem(oldArmor.id, 1);
        this->status.Speed -= oldArmor.Speed;
        this->status.DEF -= oldArmor.DEF;
        this->status.ATK -= oldArmor.ATK;
        this->armor = Armor();
        cout << "卸下了" << oldArmor.nameCN << endl;
        return true;
    }
    for (auto iter = armorBag.items.begin(); iter != armorBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->armor = *iter;
            armorBag.deleteItem((*iter).id, 1);
            // 处理加成
            this->status.ATK += ((*iter).ATK - oldArmor.ATK);
            this->status.DEF += ((*iter).DEF - oldArmor.DEF);
            this->status.Speed += ((*iter).Speed - oldArmor.Speed);
            cout << "防具" << (*iter).nameCN << "装备成功" << endl;
            return true;
        }
    }
    cout << "无该装备" << endl;
    return false;
}

/*
 * @brief 装备武器
 * 卸下武器就是装备空武器
 * @param 命令行传入的字符串name
 */
bool Player::equipWeapon(string& name) {
    Weapon oldWeapon = this->weapon;
    if (name == "None"){
        if (oldWeapon.id == 0){
            cout << "当前没有装备" << endl;
            return false;
        }
        weaponBag.addItem(oldWeapon.id, 1);
        this->status.ATK -= oldWeapon.ATK;
        this->status.DEF -= oldWeapon.DEF;
        this->status.Speed -= oldWeapon.Speed;
        this->weapon = Weapon();
        cout << "卸下了" << oldWeapon.nameCN << endl;
        return true;
    }
    for (auto iter = weaponBag.items.begin(); iter != weaponBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->weapon = *iter;
            weaponBag.deleteItem((*iter).id, 1);
            // 处理加成
            this->status.ATK += ((*iter).ATK - oldWeapon.ATK);
            this->status.DEF += ((*iter).DEF - oldWeapon.DEF);
            this->status.Speed += ((*iter).Speed - oldWeapon.Speed);
            cout << "武器" << (*iter).nameCN << "装备成功" << endl;
            return true;
        }
    }
    cout << "无该装备" << endl;
    return false;
}

/*
 * @brief 展示防具背包
 */
void Player::showArmors() {
    cout << "玩家的防具背包" << endl;
    armorBag.showItems();
}

/*
 * @brief 展示武器背包
 */
void Player::showWeapons() {
    cout << "玩家的武器背包" << endl;
    weaponBag.showItems();
}
/*
 * @brief 展示药物背包
 */
void Player::showDrugs() {
    cout << "玩家的药物" << endl;
    drugBag.showItems();
}
/*
 * @brief 展示物品背包
 */
void Player::showItems() {
    cout << "玩家的道具" << endl;
    itemBag.showItems();
}
/*
 * @brief 接受任务,同时检查是否已经接受过该任务
 *
 * @param mission:任务的引用
 * @return 是否接受成功
 */
bool Player::addMission(Mission &mission) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == mission.id){
            cout << "任务进行中" << endl;
            return false;
        }
    }
    quests.push_back(mission);
    cout << "接受任务成功" << endl;
    return true;
}
/*
 * @brief 提示任务信息
 */
void Player::showMissions() {
    if(quests.empty()){
        cout << "没有接任何任务" << endl;
        return;
    }
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        cout << "任务" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
        cout << "进度:";
        if ((*iter).isProcess){
            cout << "进行中" << endl;
        }
        if((*iter).isFinished){
            cout << "完成" << endl;
        }
    }
}
/*
 * @brief 展示单个任务的详细的信息
 *
 * @param missionId: 任务的id
 */
void Player::showMission(int missionId) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == missionId){
            cout << "任务" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
            cout << "进度:";
            if ((*iter).isProcess){
                cout << "进行中" << endl;
                //TODO:任务的描述
            }
            if((*iter).isFinished){
                cout << "完成" << endl;
            }

        }
    }
}
/*
 * @brief 获取任务
 *
 * @param assigner: 委托人的id
 */
Mission* Player::getMission(string assignerId) {
    for (auto iter = quests.begin(); iter != quests.end() ; iter++) {
        if ((*iter).assigner == assignerId){
            return &(*iter);
        }
    }
    cout << "未找到指定任务" << endl;
    return nullptr;
}
/*
 * @brief 展示玩家的信息
 */
void Player::showStatus() {
    cout << "玩家姓名:" << this->nameCN << endl;
    cout << "等级:" << this->Lv << endl;
    cout << "生命:" << this->status.HP;
    cout << "法力值:" << this->status.MP;
    cout << "速度:" << this->status.Speed;
    cout << "防御:" << this->status.DEF;
    cout << "攻击:" << this->status.ATK << endl;
}

/*
 * @brief 个人信息面板
 */
void Player::playerMenu() {
    cout << "玩家菜单" << endl;
    cout << "你可以使用下面的命令查看个人的信息" << endl;
}

/*
 * @brief 添加物品
 * 根据id分类添加
 * @param itemId:物品的id number:要添加的个数
 */
void Player::addItem(int itemId, int number) {
    int division = itemId / 100;
    if (division == 0){
        weaponBag.addItem(itemId, number);
    }
    else if (division == 1){
        armorBag.addItem(itemId, number);
    }
    else if (division == 2){
        drugBag.addItem(itemId, number);
    }
    else if (division == 3){
        itemBag.addItem(itemId, number);
    }
    else{
        cout << "错误的id" << endl;
    }
}
/*
 * @brief 根据id分类删除
 *
 * @param itemId:物品id number:要删除的个数
 */
void Player::deleteItem(int itemId, int number) {
    int division = itemId / 100;
    if (division == 0){
        weaponBag.deleteItem(itemId, number);
    }
    else if (division == 1){
        armorBag.deleteItem(itemId, number);
    }
    else if (division == 2){
        drugBag.deleteItem(itemId, number);
    }
    else if (division == 3){
        itemBag.deleteItem(itemId, number);
    }
    else{
        cout << "错误的id" << endl;
    }
}
/*
 * @brief 完全删除某个物品
 * 用于命令行的丢弃命令
 * @param itemId: 物品的id
 */
void Player::eraseItem(int itemId) {
    int division = itemId / 100;
    if (division == 0){
        weaponBag.deleteItem(itemId);
    }
    else if (division == 1){
        armorBag.deleteItem(itemId);
    }
    else if (division == 2){
        drugBag.deleteItem(itemId);
    }
    else if (division == 3){
        itemBag.deleteItem(itemId);
    }
    else{
        cout << "错误的id" << endl;
    }
}
/*
 * @brief 用于获取指定id物品的个数
 *
 * @param itemId: 物品的id
 * @return 物品的个数,0表示没有
 */
int Player::getItem(int itemId) {
    int division = itemId / 100;
    if (division == 0){
        for (unsigned int i = 0; i < weaponBag.items.size(); i++) {
            if (weaponBag.items[i].id == itemId){
                return weaponBag.items[i].num;
            }
        }
    }
    else if (division == 1){
        for (unsigned int i = 0; i < armorBag.items.size(); i++) {
            if (armorBag.items[i].id == itemId){
                return armorBag.items[i].num;
            }
        }
    }
    else if (division == 2){
        for (unsigned int i = 0; i < drugBag.items.size(); i++) {
            if (drugBag.items[i].id == itemId){
                return drugBag.items[i].num;
            }
        }
    }
    else if (division == 3){
        for (unsigned int i = 0; i < itemBag.items.size(); i++) {
            if (itemBag.items[i].id == itemId){
                return itemBag.items[i].num;
            }
        }
    }
    else{
        cout << "错误的id" << endl;
    }
    return 0;
}
/*
 * @brief 判断是否死亡
 * 可以考虑扩展一些东西
 * @return 是否死亡
 */
bool Player::isDead() {
    if (this->status.HP <= 0){
        char input;
        cout << "你处于濒死状态" << endl;
        cout << "要复活吗(y/n):";
        cin >> input;
        if (input == 'y'){
            this->status.HP += 100;
            return false;
        }
        else{
            return true;
        }
    }
    return false;
}

void Player::deadScene() {
    cout << "你死了" << endl;
    cout << "菜" << endl;
}
// -----------------------NPC类-----------------------

NPC::NPC(string id):Character() {
    this->id = std::move(id);
    //TODO:读取文件初始化
    this->shopStatus = false;
    this->battleStatus = false;
    this->missionStatus = false;
    this->isVisible = true;
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
            return true;
        }
    }
    return false;
}