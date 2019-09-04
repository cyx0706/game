//
// Created by cyx on 2019/8/30.
//
#include <utility>
#include <fstream>
#include "Character.h"
#include "Status.h"
#include "Item.h"
#include "Tool.h"
#include "templateHeader.h"
#include "global.h"

// ----------------------Character��----------------------

/*
 * @brief �����Ĺ��캯��
 *
 * @param status:״̬
 *        id:��ʶ
 *        nameCN:������
 *        nameEN:Ӣ����
 *        fallingExp:���侭��
 *        description:����
 *        location:λ��
 *        display:��ͼ�ϵ���ʾ
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
    this->id = id;
    this->nameEN = nameEN;
    this->nameCN = nameCN;
    this->description = description;
    this->mapLocation = location;
    this->displayChar = display;
    this->fallingExp = fallingExp;
    this->fallingMoney = fallingMoney;
}

/*
 * @brief ���׵Ĺ��캯��
 * ���еĳ�ʼ��Ϊ��
 *
 */
Character::Character():mapLocation({1,1,1}) {
    Status status1;
    this->status = status1;
    this->nameCN = "";
    this->nameEN = "";
    this->fallingMoney = 0;
    this->fallingExp = 0;
    this->description = "";
    this->displayChar = '\0';
}

/*
 * @brief �ж��Ƿ�����
 *
 * @return ����bool����, true��ʾ����
 */
bool Character::isDead() {
    return (this->status.HP < 0);
}

/*
 * @brief չʾ��Ϣ
 *
 */
void Character::showDescription() {
    cout << this->description << endl;
}

// ---------------------Monster��-----------------------

Monster::Monster(string id) :Character(){
    this->id = id;
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
    this->displayChar = fromString<char>(data["displayChar"]);

    string items = data["fallingItem"];
    if (items != "None"){
        vector<string>temp;
        temp = Tool::split(items, ' '); // �ո����
        for (auto iter = temp.begin(); iter != temp.end(); iter++) {
            Item aItem(fromString<int>(*iter));
            this->fallingItem.push_back(aItem);
        }
    }
    f.close();
    status.loadStatus(id,READ_MONSTER_STATUS_PATH);
}


// --------------------Player��-------------------------

/*
 * @brief ���캯��
 */
Player::Player() :Character(){
    this->id = "PY-00"; // �̶���id
    this->experiencePoint = 0;
    this->days = 0;
    this->money = 0;
    this->Lv = 1;
    this->weapon = Weapon();
    this->armor = Armor();
    this->maxMP = 150;
    this->maxMP = 100;
    this->status.HP = 150;
    this->status.MP = 100;
    this->status.ATK = 20;
    this->status.DEF = 20;
    this->status.Speed = 5;
    this->status.Critical = 50;
    this->status.Phy = 5;
}

/*
 * @brief ������ҵ�Ǯ
 *
 * @param addition:��������
 */
void Player::addMoney(int addition) {
    if (money < 100000){
        money += addition;
    }
    else{
        cout << "��Ǯ��������" << endl;
    }
}

/*
 * @brief ���Ӿ���,˳�����Ƿ�������
 *
 * @param addition:��������
 */
void Player::addExp(int addition) {
    if(Lv > 20){
        cout << "�����޷����Ӿ���" << endl;
        return;
    }
    experiencePoint += addition;
    levelUp();
    cout << "��ǰ�ȼ�" << Lv << endl;
}
/*
 * @brief ��������
 * ����������������
 * @return �Ƿ�����
 */
void Player::levelUp() {
    int nextLvExp = 100 * (Lv + 1);
    while (experiencePoint >= nextLvExp){
        experiencePoint -= nextLvExp;
        cout << nameCN << "������" << endl;
        Lv += 1;
        if (Lv > 20){
            cout << "�ﵽ����ߵȼ�" << endl;
        }
        this->status.DEF += 5;
        this->status.ATK += 5;
        this->maxHP += 15;
        this->maxMP += 15;
        this->status.Speed += 1;
        this->status.HP = maxHP;
        this->status.MP = maxMP;
        if (Lv == 3){
            // ѧϰ�µļ���
            cout << "ѧ�����µļ���" << endl;
            Skill newSkill("B04");
            this->skills.push_back(newSkill);
        }
        nextLvExp = 100 * (Lv + 1);
    }
    cout << "ʣ�� " << nextLvExp - experiencePoint << " ����������һ��" << endl;
}

/*
 * @brief װ������
 * ж�·��߾���װ���շ���
 * @param �����д�����ַ���name
 * ��name=Noneʱ��ʾж�·���
 */
bool Player::equipArmor(string &name) {
    Armor oldArmor = this->armor;
    // ж��װ��
    if( name == "None"){
        if (oldArmor.id == 0){
            cout << "��ǰû��װ��" << endl;
            return false;
        }
        armorBag.addItem(oldArmor.id, 1);
        this->status.Speed -= oldArmor.Speed;
        this->status.DEF -= oldArmor.DEF;
        this->status.ATK -= oldArmor.ATK;
        this->armor = Armor();
        cout << "ж����" << oldArmor.nameCN << endl;
        return true;
    }
    for (auto iter = armorBag.items.begin(); iter != armorBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->armor = *iter;
            armorBag.deleteItem((*iter).id, 1);
            // ����ӳ�
            this->status.ATK += ((*iter).ATK - oldArmor.ATK);
            this->status.DEF += ((*iter).DEF - oldArmor.DEF);
            this->status.Speed += ((*iter).Speed - oldArmor.Speed);
            cout << "����" << (*iter).nameCN << "װ���ɹ�" << endl;
            return true;
        }
    }
    cout << "�޸�װ��" << endl;
    return false;
}

/*
 * @brief װ������
 * ж����������װ��������
 * @param �����д�����ַ���name
 */
bool Player::equipWeapon(string& name) {
    Weapon oldWeapon = this->weapon;
    if (name == "None"){
        if (oldWeapon.id == 0){
            cout << "��ǰû��װ��" << endl;
            return false;
        }
        weaponBag.addItem(oldWeapon.id, 1);
        this->status.ATK -= oldWeapon.ATK;
        this->status.DEF -= oldWeapon.DEF;
        this->status.Speed -= oldWeapon.Speed;
        this->weapon = Weapon();
        cout << "ж����" << oldWeapon.nameCN << endl;
        return true;
    }
    for (auto iter = weaponBag.items.begin(); iter != weaponBag.items.end() ; iter++) {
        if ((*iter).nameEN == name){
            this->weapon = *iter;
            weaponBag.deleteItem((*iter).id, 1);
            // ����ӳ�
            this->status.ATK += ((*iter).ATK - oldWeapon.ATK);
            this->status.DEF += ((*iter).DEF - oldWeapon.DEF);
            this->status.Speed += ((*iter).Speed - oldWeapon.Speed);
            cout << "����" << (*iter).nameCN << "װ���ɹ�" << endl;
            return true;
        }
    }
    cout << "�޸�װ��" << endl;
    return false;
}

/*
 * @brief ������Ʒչʾ��Ʒ����ϸ��Ϣ
 */
bool Player::showItem(int itemId) {
    if (itemId < 100){
        // ����
        for (auto iter = weaponBag.items.begin(); iter != weaponBag.items.end() ; iter++) {
            if (iter->id == itemId){
                iter->showDescription();
                return true;
            }
        }
        return false;
    }
    if (itemId < 200){
        // ����
        for (auto iter = armorBag.items.begin(); iter != armorBag.items.end(); iter++) {
            if (iter->id == itemId){
                iter->showDescription();
                return true;
            }
        }
        return false;
    }
    if (itemId < 300){
        // ҩƷ
        for (auto iter = drugBag.items.begin(); iter != drugBag.items.end(); iter++) {
            if (iter->id == itemId){
                iter->showDescription();
                return true;
            }
        }
        return false;
    }
    if (itemId < 400){
        // ��Ʒ
        for (auto iter = itemBag.items.begin(); iter != itemBag.items.end() ; iter++) {
            if (iter->id == itemId){
                iter->showDescription();
                return true;
            }
        }
        return false;
    }
    return false;
}

/*
 * @brief չʾ���߱���
 */
void Player::showArmors() {
    cout << "��ҵķ��߱���" << endl;
    armorBag.showItems();
}

/*
 * @brief չʾ��������
 */
void Player::showWeapons() {
    cout << "��ҵ���������" << endl;
    weaponBag.showItems();
}
/*
 * @brief չʾҩ�ﱳ��
 */
void Player::showDrugs() {
    cout << "��ҵ�ҩ��" << endl;
    drugBag.showItems();
}

/*
 * @brief չʾ��Ʒ����
 */
void Player::showItems() {
    cout << "��ҵĵ���" << endl;
    itemBag.showItems();
}
/*
 * @brief ��������,ͬʱ����Ƿ��Ѿ����ܹ�������
 *
 * @param mission:���������
 * @return �Ƿ���ܳɹ�
 */
bool Player::addMission(Mission &mission) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == mission.id){
            cout << "���������" << endl;
            return false;
        }
    }
    quests.push_back(mission);
    cout << "��������ɹ�" << endl;
    return true;
}
/*
 * @brief ��ʾ������Ϣ
 */
void Player::showMissions() {
    if(quests.empty()){
        cout << "û�н��κ�����" << endl;
        return;
    }
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        cout << "����" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
        cout << "����:";
        if ((*iter).isProcess){
            cout << "������" << endl;
        }
        if((*iter).isFinished){
            cout << "���" << endl;
        }
    }
}
/*
 * @brief չʾ�����������ϸ����Ϣ
 *
 * @param missionId: �����id
 */
void Player::showMission(int missionId) {
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).id == missionId){
            cout << "����" << (*iter).id << (*iter).nameCN << "(" << (*iter).nameEN << ")" << endl;
            cout << "����:";
            if ((*iter).isProcess){
                cout << "������" << endl;
                //TODO:���������
            }
            if((*iter).isFinished){
                cout << "���" << endl;
            }

        }
    }
}
/*
 * @brief ��ȡ����
 * ��ȡ������ǰ���δ��ɵ�����
 * @param assigner: ί���˵�id
 */
Mission* Player::getMission(string assignerId) {
    for (auto iter = quests.begin(); iter != quests.end() ; iter++) {
        if ((*iter).assigner == assignerId && !((*iter).isFinished)){
            return &(*iter);
        }
    }
    return nullptr;
}

Mission* Player::getMission(int missionId) {
    for (auto iter = quests.begin(); iter != quests.end() ; iter++) {
        if ((*iter).id == missionId){
            return &(*iter);
        }
    }
    return nullptr;
}
/*
 * @brief չʾ��ҵ���Ϣ
 */
void Player::showStatus() {
    cout << "�������:" << this->nameCN << endl;
    cout << "�ȼ�:" << this->Lv << endl;
    cout << "����:" << this->status.HP;
    cout << "����ֵ:" << this->status.MP;
    cout << "�ٶ�:" << this->status.Speed;
    cout << "����:" << this->status.DEF;
    cout << "����:" << this->status.ATK << endl;
}

/*
 * @brief ������Ϣ���
 */
void Player::playerMenu() {
    cout << "��Ҳ˵�" << endl;
    cout << "�����ʹ�����������鿴���˵���Ϣ" << endl;
}

/*
 * @brief �����Ʒ
 * ����id�������
 * @param itemId:��Ʒ��id number:Ҫ��ӵĸ���
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
        cout << "�����id" << endl;
    }
}
/*
 * @brief ����id����ɾ��
 *
 * @param itemId:��Ʒid number:Ҫɾ���ĸ���
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
        cout << "�����id" << endl;
    }
}
/*
 * @brief ��ȫɾ��ĳ����Ʒ
 * ���������еĶ�������
 * @param itemId: ��Ʒ��id
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
        cout << "�����id" << endl;
    }
}
/*
 * @brief ���ڻ�ȡָ��id��Ʒ�ĸ���
 *
 * @param itemId: ��Ʒ��id
 * @return ��Ʒ�ĸ���,0��ʾû��
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
        cout << "�����id" << endl;
    }
    return 0;
}
/*
 * @brief �ж��Ƿ�����
 * ���Կ�����չһЩ����
 * @return �Ƿ�����
 */
bool Player::isDead() {
    if (this->status.HP <= 0){
        string input;
        cout << "�㴦�ڱ���״̬" << endl;
        cout << "Ҫ������(y/n):";
        cin >> input;
        if (input == "y"){
            this->status.HP += 100;
            return false;
        }
        else{
            return true;
        }
    }
    return false;
}
/*
 * @brief ��ҵĴ浵����
 */
void Player::save() {
    ofstream of;
    of.open(SAVE_PLAYER_PATH);
    map<string,string> m_map;

    //����player�ĵ�������
    m_map["type"] = "attribute";
    m_map["id"] = id;
    m_map["nameCN"] = nameCN;
    m_map["nameEN"] = nameEN;
    m_map["description"] = description;
    m_map["fallingExp"] = toString<int>(fallingExp);
    m_map["fallingMoney"] = toString<int>(fallingMoney);
    m_map["displayChar"] = toString<char>(displayChar);
    m_map["experiencePoint"] = toString<int>(experiencePoint);
    m_map["days"] = toString<int>(days);
    m_map["money"] = toString<int>(money);
    m_map["Lv"] = toString<int>(Lv);
    auto iter = m_map.begin();
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    m_map.clear();
    of << endl;

    //����player��location
    m_map["type"] = "location";
    m_map["mapId"] = toString<int>(mapLocation.mapId);
    m_map["x"] = toString<int>(mapLocation.x);
    m_map["y"] = toString<int>(mapLocation.y);
    for(; iter != m_map.end(); iter ++){
        of << iter->first << " " << iter->second << endl;
    }
    m_map.clear();
    of << endl;

    //����player��Status
    status.saveStatus("player");

    //����player��skills
    of << "type" << " " << "skill" << endl;
    for (auto & skill : skills) {
        of << skill.id << endl;
    }
    of << endl;

    //����player��weapon
    of << "type" << " " << "weapon" << endl;
    for (auto & item : weaponBag.items) {
        of << item.id << endl;
    }
    of << endl;

    //����player��weapon
    of << "type" << " " << "armor" << endl;
    for (auto & item : armorBag.items) {
        of << item.id << endl;
    }
    of << endl;

    //����player��drug
    of << "type" << " " << "drug" << endl;
    for (auto & item : armorBag.items) {
        of << item.id << endl;
    }
    of << endl;

    //����player��item
    of << "type" << " " << "item" << endl;
    for (auto & item : itemBag.items) {
        of << item.id << endl;
    }
    of << endl;

    of.close();
}
/*
 * @brief buffֵ�ۼ�����ҵ�����
 *
 * @param buff:������
 */
void Player::addBuff(Buff &buff) {
    if (buffs.empty()){
        buffs.push_back(buff);
    }
    else{
        // ����buff,���ж��Ƿ��ص�
        for (auto iter = buffs.begin(); iter != buffs.end() ; iter++) {
            if ((*iter).name == buff.name){
                (*iter).duration += buff.duration - 1;
                return;
            }
        }
        // ���ص�
        buffs.push_back(buff);
    }
    this->status.Critical += buff.Critical;
    this->status.Speed += buff.Speed;
    this->status.ATK += buff.ATK;
    this->status.DEF += buff.DEF;
    this->status.HP += buff.HP;
}

/*
 * @brief buffֵ������ҵ�����
 *
 * @param buff:������
 */
void Player::deleteBuff(Buff &buff) {
    this->status.Critical -= buff.Critical;
    this->status.Speed -= buff.Speed;
    this->status.ATK -= buff.ATK;
    this->status.DEF -= buff.DEF;
    this->status.HP -= buff.HP;
}

/*
 * @brief ����������ʹ��
 *
 * @param ����һ��ҩ�������
 */
bool Player::useDrug(string& name, Character &character) {
    for (auto iter = drugBag.items.begin(); iter != drugBag.items.end(); iter++) {
        if ((*iter).nameEN == name){
            this->drugBag.deleteItem((*iter).id);
            if((*iter).playerTarget){
                this->status.HP += (*iter).HP;
                if (status.HP > maxHP){
                    status.HP = maxHP;
                }
                cout << "�ָ�����" << status.HP << "Ѫ" << endl;
                this->status.MP += (*iter).MP;
                if (status.MP > maxMP){
                    status.MP = maxMP;
                }
                cout << "�ָ�����" << status.MP << "��" << endl;
            }
            else{
                character.status.HP -= iter->HP;
                cout << "���" << iter->HP << "���˺�" << endl;
            }
            return true;
        }
    }
    return false;
}
/*
 * @brief ��������ĳ���
 */
void Player::deadScene() {
    cout << "������" << endl;
    cout << "��" << endl;
}

void Player::load() {
    ifstream os;
    os.open(SAVE_PLAYER_PATH);

    string str;

    //��ȡ��������ֵ
    // �ҵ���Ӧ type Ϊ attribute ��
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "attribute") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮������ݶ�ȡΪ��ֵ��
    map<string, string> data = Tool::dataMap(os);

    this->id = id;
    this->nameCN = data["nameCN"];
    this->nameEN = data["nameEN"];
    this->description = data["description"];
    this->fallingExp = fromString<int>(data["fallingExp"]);
    this->fallingMoney = fromString<int>(data["fallingMoney"]);
    this->displayChar = fromString<char>(data["displayChar"]);
    this->experiencePoint = fromString<int>(data["experiencePoint"]);
    this->days = fromString<int>(data["days"]);
    this->money = fromString<int>(data["money"]);
    this->Lv = fromString<int>(data["Lv"]);
    data.clear();

    //��ȡλ��
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "location") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮������ݶ�ȡΪ��ֵ��
    data = Tool::dataMap(os);
    os.close();
}

void Player::battleBagShow(SCOORD& pos) {
    if (!this->drugBag.items.empty()){
        for (auto iter = drugBag.items.begin(); iter != drugBag.items.end() ; iter++) {
            Map::gotoxy(pos);
            cout << iter->nameCN << "\t" << iter->nameEN << "\t" << " X " << iter->num;
            pos.Y++;
        }
    }
    else{
        Map::gotoxy(pos);
        cout << "\t\t����Ʒ" << endl;
        pos.Y++;
    }

}
/*
 * @brief չʾ��ҵļ��ܵ���Ϣ
 */
void Player::showSkills() {
    for (auto iter = skills.begin(); iter != skills.end() ; iter++) {
        cout << iter->nameCN << "(" << iter->nameEN << ")" << endl;
    }
}
// -----------------------NPC��-----------------------

NPC::NPC(string id):Character() {
    this->id = std::move(id);
    //TODO:��ȡ�ļ���ʼ��
    this->shopStatus = false;
    this->battleStatus = false;
    this->missionStatus = false;
    this->isVisible = true;
}

/*
 * @brief �û��˵�
 */
void NPC::NPCMenu() {
    cout << this->nameCN << ":" << endl;
    if (this->bar){
        cout << "�������������Ϣ" << endl;
    }
    if (this->shopStatus){
        cout << "���Խ���" << endl;
    }
    if (this->missionStatus){
        cout << "���Խ�����" << endl;
    }
}

/*
 * @brief ��npc���׵ĺ���,��Ҫ��ǰ����Ƿ���Խ���
 *
 * @param itemId:��Ʒid number:������� Player:��ҵ�����
 */
void NPC::buy(int itemId, int number, Player &player) {
    if (!this->shopStatus){
        cout << "�޷�����" << endl;
    }
    if (this->store.buy(itemId, number, player.money)){
        cout << "����ɹ�" << endl;
        player.addItem(itemId, number);
    }
    else{
        cout << "����ʧ��" << endl;
    }
}

/*
 * @brief ������Ʒ�ĺ���
 *
 * @param itemId:��Ʒid number:������� Player:��ҵ�����
 */
void NPC::sell(Item &item, int number, Player &player) {
    if (!this->shopStatus){
        cout << "�޷�����" << endl;
    }
    this->store.sell(item, number, player.money);
}

/*
 * @brief �ж��Ƿ�����
 * ���������Ϊ���ɼ�
 */
bool NPC::isDead() {
    if (this->status.HP <= 0){
        this->isVisible = false;
        return true;
    }
    return false;
}

/*
 * @brief ��̬��չʾ����
 */
void NPC::showDescription() {
    cout << "NPC:" << this->nameCN << "(" << this->nameEN << ")";
}

/*
 * @brief ��������, ���npc�����б�ĵ�һ���Ƿ񱻽���
 *
 * @param player ��ҵ�����
 */
void NPC::assignQuest(Player &player) {
    if (!missionStatus){
        cout << "�޷���������" << endl;
        return;
    }
    for (unsigned int i = 0; i < questList.size(); i++) {
        //��һ��δ���ܵ�����
        if(!questList[i].isAccepted){
            if (!player.addMission(questList[i])){
                cout << "������ܹ���" << endl;
                return;
            }
            else{
                // ������ʱ�ĶԻ�
                cout << talkContent[questList[i].id][0] << endl;
                cout << "����������" << endl;
                return;
            }
        }
    }
    cout << "û��������Խ���" << endl;
}

/*
 * @brief �ύ����
 *
 * @param player:��ҵ�����
 */
void NPC::finishQuest(Player &player) {
    if (!missionStatus){
        cout << "�޷��ύ����" << endl;
    }
    // ��ȡ����
    Mission* acceptedMission = player.getMission(this->id);
    // �ж��Ƿ��ȡ�ɹ�
    if (acceptedMission == nullptr){
        cout << "û��Ҫ�ύ������" << endl;
    }
    else{
        // ����Ƿ�����ύ
        // ��������Ҫ�޸���������Ϊ���
        // ��������ͬʱ�޸���npc�����������
        acceptedMission->checkFinished();
    }
}

/*
 * @brief ����npc�Ƿ��ڵ�ͼ������
 */
void NPC::setVisibility(bool isVisible) {
    this->isVisible = isVisible;
}

/*
 * @brief ��ȡnpc�Ƿ�����
 * ��ͼ������Ҫ����
 */
bool NPC::getVisibility() {
    return this->isVisible;
}

/*
 * @brief ��NPC�ĶԻ�
 * ��ͬ����Ľ��н׶εĶԻ���ͬ
 */
void NPC::talk(Player &player) {
    // ���Է�����Ĳ��в�ͬ�ĶԻ�
    if (missionStatus){
        Mission *mission = player.getMission(this->id);
        if (mission != nullptr){
            int id = mission->id;
            if(mission->isProcess){
                // ������ʱ�ĶԻ�
                cout << talkContent[id][1] << endl;
                return;
            }
        }
    }
    // ƽʱ̸��
    cout << talkContent[0][0] << endl;
}

/*
 * @brief ���ڼ���Ƿ��޷��Ի�ֱ�ӽ���ս��
 *
 * @param player:��ҵ�����
 * @return �Ƿ�ᷢ��ս��
 */
bool NPC::forceBattleCheck(Player &player) {
    // ӵ�лʳ�ͨ��֤
    if (player.getItem(302) != 0){
        // ���npcս��״̬
        if (this->battleStatus){
            cout << "û��ʲô��˵��" << endl;
            return true;
        }
    }
    return false;
}