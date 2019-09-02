//
// Created by cyx on 2019/8/30.
//
#include "Character.h"

#include <utility>
#include <fstream>
#include "Item.h"
#include "Status.h"
#include "Tool.h"

// ----------------------Character??----------------------

/*
 * @brief ???????????
 *
 * @param status:??
 *        id:???
 *        nameCN:??????
 *        nameEN:?????
 *        fallingExp:???侭??
 *        description:????
 *        location:λ??
 *        display:?????????
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
 * @brief ??????????
 * ???е????????
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
 * @brief ?ж????????
 *
 * @return ????bool????, true???????
 */
bool Character::isDead() {
    return (this->status.HP < 0);
}

/*
 * @brief ?????
 *
 */
void Character::showDescription() {
    cout << this->description << endl;
}

// ---------------------Monster??-----------------------

Monster::Monster(string id) :Character(){
    // TODO:???????????
    this->id = std::move(id);
    ifstream f(READ_MONSTER_PATH);
    string str;

    while (getline(f, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && idLine[1] == id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(f);

    this->nameCN = data["nameCN"];
    this->description = data["description"];
    this->nameEN = data["nameEN"];
    this->fallingMoney = fromString<int>(data["fallingMoney"]);
    this->fallingExp = fromString<int>(data["fallingExp"]);
    //this->displayChar = data["displayChar"];

    f.close();
}


// --------------------Player??-------------------------

/*
 * @brief ??????
 */
Player::Player() :Character(){
    this->id = "PY-00"; // ?????id
    this->experiencePoint = 0;
    this->days = 0;
    this->money = 0;
    this->Lv = 1;
    this->weapon = Weapon();
    this->armor = Armor();
    // ???????±???
//    this->weaponBag = Package<Weapon>();
//    this->armorBag = Package<Armor>();
//    this->drugBag = Package<Drug>();
//    this->itemBag = Package<Item>();
}

/*
 * @brief ?????????
 *
 * @param addition:????????
 */
void Player::addMoney(int addition) {
    if (money < 100000){
        money += addition;
    }
    else{
        cout << "???????????" << endl;
    }
}

/*
 * @brief ???????,??????????????
 *
 * @param addition:????????
 */
void Player::addExp(int addition) {
    if(Lv > 20){
        cout << "??????????????" << endl;
        return;
    }
    experiencePoint += addition;
    levelUp();
    cout << "??????" << Lv << endl;
}
/*
 * @brief ????????
 * ????????????????
 * @return ???????
 */
void Player::levelUp() {
    int nextLvExp = 100 * (Lv + 1);
    while (experiencePoint > nextLvExp){
        experiencePoint -= nextLvExp;
        cout << nameCN << "??????" << endl;
        Lv += 1;
        if (Lv == 3){
            // ????????
            cout << "???????????" << endl;
        }
        nextLvExp = 100 * (Lv + 1);
    }
    cout << "??? " << nextLvExp - experiencePoint << " ?????????????" << endl;
}
/*
 * @brief ???????
 * ж?·??????????????
 * @param ?????д?????????name
 * ??name=None????ж?·???
 */
bool Player::equipArmor(string &name) {
    Armor oldArmor = this->armor;
    // ж?????
    if( name == "None"){
        if (oldArmor.id == 0){
            cout << "?????????" << endl;
            return false;
        }
        armorBag.addItem(oldArmor.id, 1);
        this->status.Speed -= oldArmor.Speed;
        this->status.DEF -= oldArmor.DEF;
        this->status.ATK -= oldArmor.ATK;
        this->armor = Armor();
        cout << "ж????" << oldArmor.nameCN << endl;
        return true;
    }
    for (auto iter = armorBag.items.begin(); iter != armorBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->armor = *iter;
            armorBag.deleteItem((*iter).id, 1);
            // ??????
            this->status.ATK += ((*iter).ATK - oldArmor.ATK);
            this->status.DEF += ((*iter).DEF - oldArmor.DEF);
            this->status.Speed += ((*iter).Speed - oldArmor.Speed);
            cout << "????" << (*iter).nameCN << "??????" << endl;
            return true;
        }
    }
    cout << "??????" << endl;
    return false;
}

/*
 * @brief ???????
 * ж???????????????????
 * @param ?????д?????????name
 */
bool Player::equipWeapon(string& name) {
    Weapon oldWeapon = this->weapon;
    if (name == "None"){
        if (oldWeapon.id == 0){
            cout << "?????????" << endl;
            return false;
        }
        weaponBag.addItem(oldWeapon.id, 1);
        this->status.ATK -= oldWeapon.ATK;
        this->status.DEF -= oldWeapon.DEF;
        this->status.Speed -= oldWeapon.Speed;
        this->weapon = Weapon();
        cout << "ж????" << oldWeapon.nameCN << endl;
        return true;
    }
    for (auto iter = weaponBag.items.begin(); iter != weaponBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->weapon = *iter;
            weaponBag.deleteItem((*iter).id, 1);
            // ??????
            this->status.ATK += ((*iter).ATK - oldWeapon.ATK);
            this->status.DEF += ((*iter).DEF - oldWeapon.DEF);
            this->status.Speed += ((*iter).Speed - oldWeapon.Speed);
            cout << "????" << (*iter).nameCN << "??????" << endl;
            return true;
        }
    }
    cout << "??????" << endl;
    return false;
}

/*
 * @brief ?????????
 */
void Player::showArmors() {
    cout << "??????????" << endl;
    armorBag.showItems();
}

/*
 * @brief ??????????
 */
void Player::showWeapons() {
    cout << "????????????" << endl;
    weaponBag.showItems();
}
/*
 * @brief ???????
 */
void Player::showDrugs() {
    cout << "???????" << endl;
    drugBag.showItems();
}
/*
 * @brief ?????????
 */
void Player::showItems() {
    cout << "???????" << endl;
    itemBag.showItems();
}
/*
 * @brief ????????,??????????????????????
 *
 * @param mission:?????????
 * @return ????????
 */
bool Player::addMission(Mission &mission) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == mission.id){
            cout << "?????????" << endl;
            return false;
        }
    }
    quests.push_back(mission);
    cout << "??????????" << endl;
    return true;
}
/*
 * @brief ??????????
 */
void Player::showMissions() {
    if(quests.empty()){
        cout << "??н??κ?????" << endl;
        return;
    }
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        cout << "????" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
        cout << "????:";
        if ((*iter).isProcess){
            cout << "??????" << endl;
        }
        if((*iter).isFinished){
            cout << "???" << endl;
        }
    }
}
/*
 * @brief ???????????????????
 *
 * @param missionId: ?????id
 */
void Player::showMission(int missionId) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == missionId){
            cout << "????" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
            cout << "????:";
            if ((*iter).isProcess){
                cout << "??????" << endl;
                //TODO:?????????
            }
            if((*iter).isFinished){
                cout << "???" << endl;
            }

        }
    }
}
/*
 * @brief ???????
 *
 * @param assigner: ??????id
 */
Mission* Player::getMission(string assignerId) {
    for (auto iter = quests.begin(); iter != quests.end() ; iter++) {
        if ((*iter).assigner == assignerId){
            return &(*iter);
        }
    }
    cout << "δ??????????" << endl;
    return nullptr;
}
/*
 * @brief ?????????
 */
void Player::showStatus() {
    cout << "???????:" << this->nameCN << endl;
    cout << "???:" << this->Lv << endl;
    cout << "????:" << this->status.HP;
    cout << "?????:" << this->status.MP;
    cout << "???:" << this->status.Speed;
    cout << "????:" << this->status.DEF;
    cout << "????:" << this->status.ATK << endl;
}

/*
 * @brief ??????????
 */
void Player::playerMenu() {
    cout << "?????" << endl;
    cout << "??????????????????????????" << endl;
}

/*
 * @brief ??????
 * ????id???????
 * @param itemId:?????id number:????????
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
        cout << "?????id" << endl;
    }
}
/*
 * @brief ????id???????
 *
 * @param itemId:???id number:?????????
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
        cout << "?????id" << endl;
    }
}
/*
 * @brief ????????????
 * ?????????е????????
 * @param itemId: ?????id
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
        cout << "?????id" << endl;
    }
}
/*
 * @brief ?????????id????????
 *
 * @param itemId: ?????id
 * @return ????????,0??????
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
        cout << "?????id" << endl;
    }
    return 0;
}
/*
 * @brief ?ж????????
 * ???????????Щ????
 * @return ???????
 */
bool Player::isDead() {
    if (this->status.HP <= 0){
        char input;
        cout << "?????????" << endl;
        cout << "???????(y/n):";
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
    cout << "??????" << endl;
    cout << "??" << endl;
}

void Player::save() {
    ofstream of;
    of.open(SAVE_TXT_PATH);
    map<string,string> m_map;
    //保存player的单项属性
    m_map["type"] = "attribute";
    m_map["id"] = id;
    m_map["fallingExp"] = std::to_string(fallingExp);
    m_map["fallingMoney"] = std::to_string(fallingMoney);
    m_map["displayChar"] = displayChar;
    m_map["experiencePoint"] = std::to_string(experiencePoint);
    m_map["days"] = std::to_string(days);
    m_map["money"] = std::to_string(money);
    m_map["Lv"] = std::to_string(Lv);
    auto iter = m_map.begin();
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    m_map.clear();
    of << endl;
    //保存player的location
    m_map["type"] = "location";
    m_map["mapId"] = std::to_string(mapLocation.mapId);
    m_map["x"] = std::to_string(mapLocation.x);
    m_map["y"] = std::to_string(mapLocation.y);
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    m_map.clear();
    of << endl;
    of.close();
    //保存player的Status
    status.saveStatus("player");
    //存buff
    for (int (i) = 0; (i) < buffs.size(); ++(i)) {
        buffs[i].saveStatus("player");
    }
    //存buff

}
// -----------------------NPC??-----------------------

NPC::NPC(string id):Character() {
    this->id = std::move(id);
    //TODO:???????????
    this->shopStatus = false;
    this->battleStatus = false;
    this->missionStatus = false;
    this->isVisible = true;
}

/*
 * @brief ??????
 */
void NPC::NPCMenu() {
    cout << this->nameCN << ":" << endl;
    if (this->bar){
        cout << "??????????????" << endl;
    }
    if (this->shopStatus){
        cout << "???????" << endl;
    }
    if (this->missionStatus){
        cout << "?????????" << endl;
    }
}

/*
 * @brief ??npc????????,??????????????????
 *
 * @param itemId:???id number:??????? money:?????
 */
void NPC::buy(int itemId, int number, int &money) {
    if (!this->shopStatus){
        cout << "???????" << endl;
    }
    this->store.buy(itemId, number, money);
}

/*
 * @brief ????????????
 *
 * @param itemId:???id number:??????? money:?????
 */
void NPC::sell(Item &item, int number, int &money) {
    if (!this->shopStatus){
        cout << "???????" << endl;
    }
    this->store.sell(item, number, money);
}

/*
 * @brief ?ж????????
 * ???????????????
 */
bool NPC::isDead() {
    if (this->status.HP <= 0){
        this->isVisible = false;
        return true;
    }
    return false;
}

/*
 * @brief ???????????
 */
void NPC::showDescription() {
    cout << "NPC:" << this->nameCN << "(" << this->nameEN << ")";
}

/*
 * @brief ????????, ???npc?????б????????????
 *
 * @param player ????????
 */
void NPC::assignQuest(Player &player) {
    if (!missionStatus){
        cout << "???????????" << endl;
        return;
    }
    for (unsigned int i = 0; i < questList.size(); i++) {
        //?????δ?????????
        if(!questList[i].isAccepted){
            if (!player.addMission(questList[i])){
                cout << "??????????" << endl;
                return;
            }
            else{
                // ???????????
                cout << talkContent[questList[i].id][0] << endl;
                cout << "??????????" << endl;
                return;
            }
        }
    }
    cout << "?????????????" << endl;
}

/*
 * @brief ??????
 *
 * @param player:????????
 */
void NPC::finishQuest(Player &player) {
    if (!missionStatus){
        cout << "?????????" << endl;
    }
    // ???????
    Mission* acceptedMission = player.getMission(this->id);
    // ?ж?????????
    if (acceptedMission == nullptr){
        cout << "????????????" << endl;
    }
    else{
        // ???????????
        // ????????????????????????
        // ???????????????npc???????????
        acceptedMission->checkFinished();
    }
}

/*
 * @brief ????npc?????????????
 */
void NPC::setVisibility(bool isVisible) {
    this->isVisible = isVisible;
}

/*
 * @brief ???npc???????
 * ??????????????
 */
bool NPC::getVisibility() {
    return this->isVisible;
}

/*
 * @brief ??NPC????
 * ??????????н?ε??????
 */
void NPC::talk(Player &player) {
    // ????????????в??????
    if (missionStatus){
        Mission *mission = player.getMission(this->id);
        if (mission != nullptr){
            int id = mission->id;
            if(mission->isProcess){
                // ???????????
                cout << talkContent[id][1] << endl;
                return;
            }
        }
    }
    // ?????
    cout << talkContent[0][0] << endl;
}

/*
 * @brief ????????????????????????
 *
 * @param player:????????
 * @return ?????????
 */
bool NPC::forceBattleCheck(Player &player) {
    // ??л??????
    if (player.getItem(302) != 0){
        // ???npc?????
        if (this->battleStatus){
            cout << "??????????" << endl;
            return true;
        }
    }
    return false;
}