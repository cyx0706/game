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
    return (this->status.HP <= 0);
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
        if (Lv == 2){
            // ѧϰ�µļ���
            cout << "ѧ�����µļ���:����̬��" << endl;
            Skill newSkill("B02");
            this->skills.push_back(newSkill);
        }
        if(Lv == 3){
            cout << "ѧ���µļ���:һ��999" << endl;
            Skill newSkill("A02");
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
 * @brief չʾ��ǰ�����е�����
 *
 * @param missionId: �����id
 */
void Player::showMission() {
    cout << "-------��ǰ�����е�����:-------" << endl;
    bool flag = false;
    for (auto iter = quests.begin(); iter != quests.end(); iter++) {
        if ((*iter).isProcess){
            flag = true;
            iter->showDescription();
        }
    }
    if (!flag){
        cout << "û�н����е�����!" << endl;
    }

}
/*
 * @brief ��ȡ����
 * ��ȡ����ί���˵ĵ�һ��δ��ɵ�����
 * @param assigner: ί���˵����ֵ�Ӣ��
 */
Mission* Player::getMission(string &assignerName) {
    if (quests.empty()){
        return nullptr;
    }
    for (auto iter = quests.begin(); iter != quests.end() ; iter++) {
        if ((*iter).assigner == assignerName && !((*iter).isFinished)){
            return &(*iter);
        }
    }
    return nullptr;
}

Mission* Player::getMission(int missionId) {
    if (quests.empty()){
        return nullptr;
    }
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
    cout << "����:\t" << this->status.HP << endl;
    cout << "����ֵ:\t" << this->status.MP;
    cout << "�ٶ�:\t" << this->status.Speed;
    cout << "����:\t" << this->status.DEF;
    cout << "����:\t" << this->status.ATK << endl;
}

/*
 * @brief ������Ϣ���
 */
//TODO:�Ż�UI
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

    //����player��armor
    of << "type" << " " << "armor" << endl;
    for (auto & item : armorBag.items) {
        of << item.id << endl;
    }
    of << endl;

    //����player��drug
    of << "type" << " " << "drug" << endl;
    for (auto & item : drugBag.items) {
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
            this->drugBag.deleteItem((*iter).id, 1);
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

//player����
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

    //��ȡλ������ֵ
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

    this->mapLocation.y = fromString<int>(data["y"]);
    this->mapLocation.x = fromString<int>(data["x"]);
    this->mapLocation.mapId = fromString<int>(data["mapId"]);
    data.clear();

    //��ȡskill
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "skill") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮���skills����id��ֵ
    while (getline(os, str)) {
        // �������н���
        if (!str.empty()) {
            skills.emplace_back(str);
        } else {
            break;
        }
    }

    //��ȡweapon
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "weapon") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮���weapon����id��ֵ
    while (getline(os, str)) {
        // �������н���
        if (!str.empty()) {
            weaponBag.addItem(fromString<int>(str),1);
        } else {
            break;
        }
    }

    //��ȡarmor
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "armor") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮���armor����id��ֵ
    while (getline(os, str)) {
        // �������н���
        if (!str.empty()) {
            armorBag.addItem(fromString<int>(str),1);
        } else {
            break;
        }
    }

    //��ȡdrug
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "drug") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮���drug����id��ֵ
    while (getline(os, str)) {
        // �������н���
        if (!str.empty()) {
            drugBag.addItem(fromString<int>(str),1);
        } else {
            break;
        }
    }

    //��ȡitem
    while (getline(os, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "type" && idLine[1] == "item") {
                break;
            }
        }
    }
    // �� ��Ӧ type �е���һ������֮���item����id��ֵ
    while (getline(os, str)) {
        // �������н���
        if (!str.empty()) {
            itemBag.addItem(fromString<int>(str),1);
        } else {
            break;
        }
    }

    os.close();

    status.loadStatus("player",SAVE_STATUS_PATH);
}

/*
 * @brief ս��ʱ��������ʾ
 */
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
        cout << iter->nameCN << "(" << iter->nameEN << ")" << iter->description << endl;
    }
}
// -----------------------NPC��-----------------------

/*
 * @brief ����npc, ���Զ��ж��Ƿ���Ҫ�����
 * �洢���񽻸���ҵĴ洢
 * �̵����ھ�̬��������
 */
void NPC::save() {
    if (this->needSave){
        ofstream fp;
        fp.open(SAVE_NPC_PATH);
        fp << "id" << " " << this->id << endl;
        fp << "nameCN" << " " << this->nameCN << endl;
        fp << "shopStatus" << " " << this->shopStatus << endl;
        fp << "missionStatus" << " " << this->missionStatus << endl;
        fp << "bar" << " " << this->bar << endl;
        fp << "isVisible" << " " << this->isVisible << endl;
        fp << "quests";
        for (unsigned int i = 0; i < questList.size(); i++) {
            fp << questList[i].id;
            if (i == questList.size()-1){
                continue;
            }
            cout << ",";
        }
        cout << endl;
        fp.close();
    }
}

/*
 * @brief ����npc�Ķ�̬������
 * ����Ҫ�ȹ���npc
 * �ļ������id��npc
 * ��Щnpc����ҪһЩ��������,�Ͱ���Ĭ�ϵ��趨��
 * <!��Ҫ>ֻ����Ҫ��ȡ��ʱ���ٵ���
 * �̵����ھ�̬��������
 */
void NPC::load() {
    // ��Ҫ��ȡ
    ifstream fp;
    fp.open(SAVE_NPC_PATH);
    string str;
    while (getline(fp, str)) {
        if (!str.empty()) {
            vector<string> idLine = Tool::split(str);
            if (idLine[0] == "id" && idLine[1] == this->id) {
                break;
            }
        }
    }
    map<string, string> data = Tool::dataMap(fp);

    this->shopStatus = fromString<bool>(data["shopStatus"]);
    this->battleStatus = fromString<bool>(data["battleStatus"]);
    this->missionStatus = fromString<bool>(data["missionStatus"]);
    this->bar = fromString<bool>(data["bar"]);
    this->isVisible = fromString<bool>(data["isVisible"]);
    vector<string>missions = Tool::split(data["quests"], ',');
    // ��ȡδ��ɵ�����
    for (auto iter = missions.begin(); iter != missions.end(); iter++) {
        Mission remainMission(fromString<int>(*iter));
        this->questList.push_back(remainMission);
    }
    fp.close();

}

/*
 * @brief ����������ȡnpc�ľ�̬��Ϣ
 * �����ļ��ڵ�key��Ҫע��:
 * -�����ϸ������ڵĸ�ʽ,�޷������޸�key
 * end��β
 * �����ļ��ڵ�talk��Ҫע��:
 * -�����ʽ <id>:<type>:<sentence>
 * -��idΪ0ʱ˵��Ϊ�ճ��Ի�,��ʱtype������ʱ����
 * -��id��Ϊ0ʱ��ʾ�����id,��ʱtype��Ӧ�ṹ��TalkContent��3���ַ���
 * -<!��Ҫ>id��˳��һ��Ҫ0�ڵ�һ��,��ͬid�Ĳ���˳��,��һ��Ҫ��һ��
 */
istream& operator>>(istream &fpStream, NPC &npc) {
    string temp;
    string line;
    vector<string>t;
    TalkContent talkContent = {"", "", ""};
    int missionId;
    fpStream >> temp >> npc.nameCN;
    fpStream >> temp >> npc.nameEN;
    fpStream >> temp >> npc.description;
    fpStream >> temp >> npc.mapLocation.x;
    fpStream >> temp >> npc.mapLocation.y;
    fpStream >> temp >> npc.mapLocation.mapId;
    fpStream >> temp >> npc.displayChar;
    fpStream >> temp >> npc.fallingExp;
    fpStream >> temp >> npc.fallingMoney;
    fpStream >> temp >> npc.needSave;
    NPC::readLastLine += 10;
    int lastId = 0;
    fpStream >> temp;
    while (temp != "end"){
        fpStream >> line;
        // ������Ϣ˵����������һ��
        NPC::readLastLine++;
        t = Tool::split(line, ':');

        missionId = fromString<int>(t[0]);
        // ��ȡ��id��ͬ, ˵�����Դ�һ����
        if (lastId != missionId){
            cout << lastId;
            npc.talkContent.insert(make_pair(lastId, talkContent));
            // ���
            talkContent = {"", "", ""};
        }

        if (missionId != 0){
            if (t[1] == "start"){
                talkContent.start = t[2];
            }
            if(t[1] == "end"){
                talkContent.end = t[2];
            }
            // TODO:������������еĶԻ�
            if (t[1] == "process"){
                talkContent.process = "��ȥ";
            }
        }
        else{
            // ƽʱ�Ի���ֱ�ӵ���start
            talkContent.start = t[2];

        }
        lastId = missionId;
        fpStream >> temp;
        if (temp == "end"){
            // ����û����
            npc.talkContent.insert(make_pair(missionId, talkContent));
            // endռһ��
            NPC::readLastLine++;
        }
    }
    return fpStream;
}


NPC::NPC(string id):Character() {
    this->id = id;
    this->needSave = false;
    ifstream fp;
    string line;
    int round = 0;
    fp.open(NPC_FILE_PATH);
    while (round < NPC::readLastLine){
        getline(fp, line);
        round++;
    }
    while (fp.peek() != EOF){
        getline(fp, line);
        readLastLine++;
        if (line == "------"){
            getline(fp, line);
            readLastLine++;
            vector<string>t = Tool::split(line, ' ');
            if (t[0] == "id" && t[1] == id){
                fp >> *this;
                fp.close();
                if (this->needSave){
                    this->load();
                }
                else{
                    // �̵�npc
                    if (id == "NN-07"){
                        this->shopStatus = true;
                    }
                    else{
                        this->shopStatus = false;
                    }
                    this->missionStatus = false;
                    this->battleStatus = false;
                    this->bar = false;
                    this->isVisible = true;
                }
                return;
            }
            else{
                continue;
            }
        }
        else{
            continue;
        }
    }
    // Ӧ���滻Ϊtry-catch����?
    fp.close();
    cout << NPC::readLastLine;
    cout << this->id;
    system("pause");
    exit(1);
}

/*
 * @brief �û��˵�
 */
void NPC::NPCMenu(Player &player) {
    // ����������
    if (this->missionStatus){
        cout << this->nameCN << endl;
        cout << this->description << endl;
        if (player.getMission(this->nameEN) != nullptr){
            cout << "��ǰ���������" << endl;
            return;
        }
        else{
            cout << "�п��Խ��ܵ�����:" << endl;
            cout << questList[0].nameCN << endl;
        }
        return;
    }

    // ���̵��״̬
    if (!this->shopStatus){
        cout << this->nameCN << endl;
        cout << this->description << endl;
        return;
    }
    // �̵����
    // �Ի�Ҫ��
    SCOORD curPos = {40, 1};
    for (int i = 0; i < 90; i++) {
        cout << "*";
    }
    curPos.X = 40;
    for (int j = 1; j <= 27; j++) {
        Map::gotoxy(curPos);
        cout << "*";
        curPos.Y++;
    }
    curPos = {0, 1};
    for (int j = 1; j < 10; j++) {
        Map::gotoxy(curPos);
        cout << "*";
        curPos.Y++;
    }
    Map::gotoxy(curPos);
    for (int k = 0; k < 40; k++) {
        cout << "*";
    }

    curPos = {18,1};
    Map::gotoxy(curPos);
    cout << this->nameCN;

    curPos = {10,6};
    Map::gotoxy(curPos);
    cout << "��ӵ�е�Ǯ: " << player.money << endl;

    curPos = {41, 1};
    Map::gotoxy(curPos);
    store.shopMenu(curPos);

    curPos = {0, 11};
    Map::gotoxy(curPos);
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
    if (store.buy(itemId, number, player.money)){
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
    store.sell(item, number, player.money);
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
        cout << "������" << endl;
        return;
    }
    if(!questList.empty()){
        if (player.getMission(this->nameEN)){
            cout << "��ǰί���˽��е�����δ���" << endl;
            return;
        }
        else {
            // �����Ѿ����ܲ��ҽ�����
            questList[0].isAccepted = true;
            questList[0].isProcess = true;
            // ������ʱ�ĶԻ�
            cout << talkContent[questList[0].id].start << endl;
            player.addMission(questList[0]);
            // ɾ���������
            questList.erase(questList.begin());

            return;
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
    Mission* acceptedMission = player.getMission(this->nameEN);
    // �ж��Ƿ��ȡ�ɹ�
    if (acceptedMission == nullptr){
        cout << "û�п����ύ������" << endl;
    }
    else{
        // ����Ƿ�����ύ
        // ��������Ҫ�޸���������Ϊ���
        if (acceptedMission->checkFinished()){
            // �������
            cout << this->talkContent[acceptedMission->id].end << endl;
            acceptedMission->isFinished = true;
            acceptedMission->isProcess = false;
            // ��� ���ӽ�Ǯ �� ����
            player.addMoney(acceptedMission->bonusMoney);
            player.addExp(acceptedMission->bonusExperiencePoint);
        }
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
 * @return true��ʾ������ʾ
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

    // ����ĶԻ���������
    if (this->id == "NN-10"){
        if (player.getItem(302) != 0){
            cout << "�������߹�, �Һ���ûʲô��˵��" << endl;
        }
    }
    if (missionStatus){
        Mission *mission = player.getMission(this->id);
        if (mission != nullptr){
            int id = mission->id;
            if(mission->isProcess){
                // ������ʱ�ĶԻ�
                cout << talkContent[id].process << endl;
                return;
            }
        }
    }
    // ƽʱ̸��, ע��ƽʱ̸����start
    cout << talkContent[0].start << endl;
}

/*
 * @brief ���ڼ���Ƿ��޷��Ի�ֱ�ӽ���ս��
 *
 * @param player:��ҵ�����
 * @return �Ƿ�ᷢ��ս��
 */

//TODO:������
bool NPC::forceBattleCheck(Player &player) {
    // ӵ�лʳ�ͨ��֤
    if (this->battleStatus){
        // ���npcս��״̬
        if (player.getItem(302) != 0){
            cout << "û��ʲô��˵��" << endl;
            return true;
        }
    }
    return false;
}