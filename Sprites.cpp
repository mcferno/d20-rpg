#include "Sprites.h"

// #####################################################################################################

Object::Object()
{
	x = y = -1;
	clip = new SDL_Rect();
	graphics = NULL;
}

// #####################################################################################################

Character::Character() : Object()
{
}

void Character::setName(char* name) {
	this->name = name;
}

char* Character::getName() {
	return this->name;
}

int Character::getBaseAttackBonus(){
	return getLevel()-1;
}

int Character::getAttackBonus(){
	return (getBaseAttackBonus() + getStrMod());
}

int Character::getInitiativeRoll(){
	return (Dice::roll(Dice::D20,1) + getDexMod());
}

int Character::rollWeaponDamage(int diceType)
{
	int damageRoll, damage;
	damageRoll = Dice::roll(diceType);
	damage = damageRoll + getStrMod();
	std::cout << "Roll Damage: Combat d " << diceType << " roll: " << damageRoll << " + Strenght Mod: " << getStrMod() << " = " << damage << "\n\n";
	return damage;
}


int Character::getRace()
{
	return imyRace;
}

int Character::getClass()
{
	return imyClass;
}

void Character::setRace(int r)
{
	imyRace = r;
}

void Character::setClass(int c)
{
	imyClass = c;
}




//BEGIN MODIFIERS
void Character::setStr(int str){
	if (str <= 0)
		this->str = 0;
	else
		this->str = str;
}
void Character::setDex(int dex){
	if (dex <= 0)
		this->dex = 0;
	else
		this->dex = dex;
}
void Character::setCon(int con){
	if (con <= 0)
		this->con = 0;
	else
		this->con = con;
}
void Character::setIte(int ite){
	if (ite <= 0)
		this->ite = 0;
	else
		this->ite = ite;
}
void Character::setWis(int wis){
	if (wis <= 0)
		this->wis = 0;
	else
		this->wis = wis;
}
void Character::setCha(int cha){
	if (cha <= 0)
		this->cha = 0;
	else
		this->cha = cha;
}
void Character::setAllModifiers(int str, int dex, int con, int ite, int wis, int cha ){
	setStr(str);
	setDex(dex);
	setCon(con);
	setIte(ite);
	setWis(wis);
	setCha(cha);
}
void Character::setHp(int hp){
	if (hp <= 0)
	{
		this->hp = 0;
		died();
	}
	else
		this->hp = hp;
}
void Character::setLevel(int level){
	if (level <= 1)
		this->level = 1;
	else
		this->level = level;
}

void Character::setSpeed(int speedInFeet){
	if (speedInFeet < 5)
		this->speed = 1;
	else
		this->speed = speedInFeet / FT_TO_TILES;
}
//END MODIFIERS

//BEGIN ACCESSORS
int Character::getStr() {
	return str;
}
int Character::getDex() {
	return dex;
}
int Character::getCon() {
	return con;
}
int Character::getIte() {
	return ite;
}
int Character::getWis() {
	return wis;
}
int Character::getCha() {
	return cha;
}
int Character::getHp(){
	return hp;
}
int Character::getLevel(){
	return level;
}
int Character::getSpeed(){
	return speed;
}

int Character::getModifier(int base)
{
	return (base/2)-5;
}

int Character::getStrMod() {
	return getModifier(str);
}
int Character::getDexMod() {
	return getModifier(dex);
}
int Character::getConMod() {
	return getModifier(con);
}
int Character::getIteMod() {
	return getModifier(ite);
}
int Character::getWisMod() {
	return getModifier(wis);
}
int Character::getChaMod() {
	return getModifier(cha);
}

// Armor Class (AC): How hard a character is to hit. 
// 10 + armor bonus + shield bonus + size modifier + dexterity modifier
int Character::getAC(){
	int ac = 10 + getModifier(dex);
	std::cout << "Calculate Armour Class:\n10 + Dex Modifier: " << getModifier(dex) << " = " << (ac) << "\n";
	return ac;
}
//END ACCESSORS

void Character::showCharacter() {
	std::cout << "\nYour character (after Race based adjustments):\n"
		<< "STR: " << getStr() << ", Mod: " << getStrMod() << "\n"
		<< "DEX: " << getDex() << ", Mod: " << getDexMod() << "\n"
		<< "CON: " << getCon() << ", Mod: " << getConMod() << "\n"
		<< "ITE: " << getIte() << ", Mod: " << getIteMod() << "\n"
		<< "WIS: " << getWis() << ", Mod: " << getWisMod() << "\n"
		<< "CHA: " << getCha() << ", Mod: " << getChaMod() << "\n"
		<< "SPEED: " << getSpeed() << "\n"
		<< "HP: " << getHp() << "\n";
}

void Character::died()
{
	if(isDead())
	{
		std::cout << name << " has dies.\n";
		speed = 0;
	}
}

bool Character::isDead()
{
	return (hp < 1);
}


// #####################################################################################################


ControllableCharacter::ControllableCharacter() : Character()
{
	rollStartingMoney();
	equippedWeapon = NULL;
	equippedShield = NULL;
	equippedHelmet = NULL;
	equippedVest = NULL;

	UsableItem** tempItems = UsableItemFactory::getAllUsableItems();

	// all characters start with 0 potions, 0 arrows and 0 bolts
	numArrows = numBolts = numPotions = 0;
	inventory.addItem(tempItems[0]);
	inventory.addItem(tempItems[1]);
	inventory.addItem(tempItems[2]);
}

int ControllableCharacter::rollWeaponDamage()
{
	int damageRoll, damage;
	char* weaponName;

	if (equippedWeapon != NULL)
		damageRoll = Dice::roll(equippedWeapon->getDamage());
	else {
		damageRoll = HAND_COMBAT_DICE;
		weaponName = "Hand Combat";
	}
	
	damage = damageRoll + getStrMod();
	std::cout << "Roll Damage: "<< weaponName << " d" << equippedWeapon->getDamage() << " roll:  " << damageRoll << " + Strenght Mod: " << getStrMod() << " = " << damage << "\n\n";
	return damage;
}

// Armor Class (AC): How hard a character is to hit. 
// 10 + armor bonus + shield bonus + size modifier + dexterity modifier
int ControllableCharacter::getAC()
{
	int vestBonus = 0;
	int helmetBonus = 0;
	int shieldBonus = 0;
	int AC;

	if (equippedShield != NULL)
		shieldBonus=equippedShield->getArmorBonus();

	if (equippedHelmet != NULL)
		helmetBonus = equippedHelmet->getArmorBonus();
	
	if (equippedVest != NULL)
		vestBonus = equippedVest->getArmorBonus();

	AC = 10 + getModifier(dex) + vestBonus + helmetBonus + shieldBonus;

	std::cout << "Armor Class Calculation: \n" << "Shield Bonus: " << shieldBonus << "+ Helmet Bonus: " << helmetBonus << "  + Vest Bonus: " << vestBonus;
	std::cout << " + Dexterity Mod: " << getModifier(dex) << "+ 10 = " << AC << "\n\n";
	return AC;
}


void ControllableCharacter::rollStartingMoney()
{
	money.addCoin(Dice::roll(STARTING_MONEY_DICE_TYPE,STARTING_MONEY_DICE_ROLLS),STARTING_MONEY_TYPE);
}

int ControllableCharacter::getAbilityRoll()
{
	int smallestRoll = 0;
	int rollAmount;
	int runningSum = 0;

	// roll the dice a number of times, calculate the running sum and locate the smallest roll
	std::cout << "Rolling: ";
	for(int i=0;i<ABILITY_ROLL_DICE_ROLLS;i++)
	{
		rollAmount = Dice::roll(ABILITY_ROLL_DICE_TYPE);
		runningSum += rollAmount;
		std::cout << rollAmount << ",";

		// track the smallest roll if it isn't the only roll to be performed
		if(i == 0 && ABILITY_ROLL_DICE_ROLLS > 1)
		{
			smallestRoll = rollAmount;
		}
		else
		{
			if(smallestRoll > rollAmount)
				smallestRoll = rollAmount;
		}
	}

	std::cout << " discarding: " << smallestRoll << ", total: " << (runningSum - smallestRoll) << "\n" ;
	
	// discard the smallest roll and return the sum
	return (runningSum - smallestRoll);
}

void ControllableCharacter::unEquip(Item* toUnequip)
{
	if(equippedHelmet == toUnequip)
		equippedHelmet = NULL;
	else if(equippedWeapon == toUnequip)
		equippedWeapon = NULL;
	else if(equippedShield == toUnequip)
		equippedShield = NULL;
	else if(equippedVest == toUnequip)
		equippedVest = NULL;
	else {}
}

void ControllableCharacter::addItem(Item* toAdd)
{
	if(toAdd->isItemOfQuantity())
	{
		UsableItem* tempUsableItem =  static_cast<UsableItem*>(toAdd);
		switch(tempUsableItem->usableType)
		{
			case UsableItem::POTION:
				numPotions += tempUsableItem->numLeft();
				break;
			case UsableItem::ARROW:
				std::cout << "Increasing the number of arrows " << tempUsableItem->numLeft();
				numArrows += tempUsableItem->numLeft();
				break;
			case UsableItem::BOLT:
				std::cout << "Increasing the number of bolts " << tempUsableItem->numLeft();
				numBolts += tempUsableItem->numLeft();
				break;
		}
	}
	else
	{
		inventory.addItem(toAdd);
	}
}

void ControllableCharacter::removeItem(Item* toRemove)
{
	unEquip(toRemove);
	inventory.removeItem(toRemove);
}

bool ControllableCharacter::isEquipped(Item* toFind)
{
	return(equippedHelmet == toFind || equippedWeapon == toFind || equippedShield == toFind || equippedVest == toFind);
}

int ControllableCharacter::getNumArrows()
{
	return numArrows;
}
int ControllableCharacter::getNumBolts()
{
	return numBolts;
}
int ControllableCharacter::getNumPotions()
{
	return numPotions;
}

void ControllableCharacter::setNumArrows(int numArrows)
{
	this->numArrows = numArrows;
}
void ControllableCharacter::setNumBolts(int numBolts)
{
	this->numBolts = numBolts;
}
void ControllableCharacter::setNumPotions(int NumPotions)
{
	this->numPotions = numPotions;
}

int ControllableCharacter::getWeaponRange()
{
	int weaponReach = 0;
	if(equippedWeapon != NULL)
		weaponReach = equippedWeapon->getRangeIncrement();
	if(weaponReach < 1)
		weaponReach = 1;
	return weaponReach;
}

// #####################################################################################################

Fighter::Fighter()
{

}

//Constructor class
//Takes a type race to specify which race the character is
//Each race may have specific additions to the modifiers
//Hitpoints, Level, and AC begin at a predetermined base
//Str, Dex, Con, Ite, Wis, Cha are all determined randomly by the character roll.
Fighter::Fighter(race myrace) : ControllableCharacter() {

	setHp(FIGHTER_HP);
	setLevel(1);
	setClass(Character::FIGHTER);

	std::cout << "\nNow rolling your abilities using a " << ABILITY_ROLL_DICE_ROLLS << "d" << ABILITY_ROLL_DICE_TYPE << " ...\n";

	switch (myrace)
	{
		case HUMAN:
			setStr(getAbilityRoll() + HumanRace::STR_ADJ);
			setDex(getAbilityRoll() + HumanRace::DEX_ADJ);
			setCon(getAbilityRoll() + HumanRace::CON_ADJ);
			setIte(getAbilityRoll() + HumanRace::ITE_ADJ);
			setWis(getAbilityRoll() + HumanRace::WIS_ADJ);
			setCha(getAbilityRoll() + HumanRace::CHA_ADJ);
			setSpeed(HumanRace::SPEED);
			setRace(Race::HUMAN);
			break;
		case DWARF:
			setStr(getAbilityRoll() + DwarfRace::STR_ADJ);
			setDex(getAbilityRoll() + DwarfRace::DEX_ADJ);
			setCon(getAbilityRoll() + DwarfRace::CON_ADJ);
			setIte(getAbilityRoll() + DwarfRace::ITE_ADJ);
			setWis(getAbilityRoll() + DwarfRace::WIS_ADJ);
			setCha(getAbilityRoll() + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			setRace(Race::DWARF);
			break;
		case ELF:
			setStr(getAbilityRoll() + ElfRace::STR_ADJ);
			setDex(getAbilityRoll() + ElfRace::DEX_ADJ);
			setCon(getAbilityRoll() + ElfRace::CON_ADJ);
			setIte(getAbilityRoll() + ElfRace::ITE_ADJ);
			setWis(getAbilityRoll() + ElfRace::WIS_ADJ);
			setCha(getAbilityRoll() + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			setRace(Race::ELF);
			break;
		case GNOME:
			setStr(getAbilityRoll() + GnomeRace::STR_ADJ);
			setDex(getAbilityRoll() + GnomeRace::DEX_ADJ);
			setCon(getAbilityRoll() + GnomeRace::CON_ADJ);
			setIte(getAbilityRoll() + GnomeRace::ITE_ADJ);
			setWis(getAbilityRoll() + GnomeRace::WIS_ADJ);
			setCha(getAbilityRoll() + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			setRace(Race::GNOME);
			break;
	}
}

Fighter::Fighter(race myrace, int str, int dex, int con, int ite, int wis, int cha) : ControllableCharacter() {

	setHp(FIGHTER_HP);
	setLevel(1);
	setClass(Character::FIGHTER);


	switch (myrace)
	{
		case HUMAN:
			setStr(str + HumanRace::STR_ADJ);
			setDex(dex + HumanRace::DEX_ADJ);
			setCon(con + HumanRace::CON_ADJ);
			setIte(ite + HumanRace::ITE_ADJ);
			setWis(wis + HumanRace::WIS_ADJ);
			setCha(cha + HumanRace::CHA_ADJ);
			setSpeed(HumanRace::SPEED);
			setRace(Race::HUMAN);
			break;
		case DWARF:
			setStr(str + DwarfRace::STR_ADJ);
			setDex(dex + DwarfRace::DEX_ADJ);
			setCon(con + DwarfRace::CON_ADJ);
			setIte(ite + DwarfRace::ITE_ADJ);
			setWis(wis + DwarfRace::WIS_ADJ);
			setCha(cha + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			setRace(Race::DWARF);
			break;
		case ELF:
			setStr(str + ElfRace::STR_ADJ);
			setDex(dex + ElfRace::DEX_ADJ);
			setCon(con + ElfRace::CON_ADJ);
			setIte(ite + ElfRace::ITE_ADJ);
			setWis(wis + ElfRace::WIS_ADJ);
			setCha(cha + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			setRace(Race::ELF);
			break;
		case GNOME:
			setStr(str + GnomeRace::STR_ADJ);
			setDex(dex + GnomeRace::DEX_ADJ);
			setCon(con + GnomeRace::CON_ADJ);
			setIte(ite + GnomeRace::ITE_ADJ);
			setWis(wis + GnomeRace::WIS_ADJ);
			setCha(cha + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			setRace(Race::GNOME);
			break;
	}
}

// #####################################################################################################
Ranger::Ranger()
{

}

//Constructor class
//Takes a type race to specify which race the character is
//Each race may have specific additions to the modifiers
//Hitpoints, Level, and AC begin at a predetermined base
//Str, Dex, Con, Ite, Wis, Cha are all determined randomly by the character roll.
Ranger::Ranger(race myrace) : ControllableCharacter() {

	setHp(RANGER_HP);
	setLevel(1);
	setClass(Character::RANGER);

	std::cout << "\nNow rolling your abilities using a " << ABILITY_ROLL_DICE_ROLLS << "d" << ABILITY_ROLL_DICE_TYPE << " ...\n";

	switch (myrace)
	{
		case HUMAN:
			setStr(getAbilityRoll() + HumanRace::STR_ADJ);
			setDex(getAbilityRoll() + HumanRace::DEX_ADJ);
			setCon(getAbilityRoll() + HumanRace::CON_ADJ);
			setIte(getAbilityRoll() + HumanRace::ITE_ADJ);
			setWis(getAbilityRoll() + HumanRace::WIS_ADJ);
			setCha(getAbilityRoll() + HumanRace::CHA_ADJ);
			setSpeed(HumanRace::SPEED);
			setRace(Race::HUMAN);
			break;
		case DWARF:
			setStr(getAbilityRoll() + DwarfRace::STR_ADJ);
			setDex(getAbilityRoll() + DwarfRace::DEX_ADJ);
			setCon(getAbilityRoll() + DwarfRace::CON_ADJ);
			setIte(getAbilityRoll() + DwarfRace::ITE_ADJ);
			setWis(getAbilityRoll() + DwarfRace::WIS_ADJ);
			setCha(getAbilityRoll() + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			setRace(Race::DWARF);
			break;
		case ELF:
			setStr(getAbilityRoll() + ElfRace::STR_ADJ);
			setDex(getAbilityRoll() + ElfRace::DEX_ADJ);
			setCon(getAbilityRoll() + ElfRace::CON_ADJ);
			setIte(getAbilityRoll() + ElfRace::ITE_ADJ);
			setWis(getAbilityRoll() + ElfRace::WIS_ADJ);
			setCha(getAbilityRoll() + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			setRace(Race::ELF);
			break;
		case GNOME:
			setStr(getAbilityRoll() + GnomeRace::STR_ADJ);
			setDex(getAbilityRoll() + GnomeRace::DEX_ADJ);
			setCon(getAbilityRoll() + GnomeRace::CON_ADJ);
			setIte(getAbilityRoll() + GnomeRace::ITE_ADJ);
			setWis(getAbilityRoll() + GnomeRace::WIS_ADJ);
			setCha(getAbilityRoll() + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			setRace(Race::GNOME);
			break;
	}
}

Ranger::Ranger(race myrace, int str, int dex, int con, int ite, int wis, int cha) : ControllableCharacter() {

	setHp(RANGER_HP);
	setLevel(1);
	setClass(Character::RANGER);


	switch (myrace)
	{
		case HUMAN:
			setStr(str + HumanRace::STR_ADJ);
			setDex(dex + HumanRace::DEX_ADJ);
			setCon(con + HumanRace::CON_ADJ);
			setIte(ite + HumanRace::ITE_ADJ);
			setWis(wis + HumanRace::WIS_ADJ);
			setCha(cha + HumanRace::CHA_ADJ);
			setSpeed(HumanRace::SPEED);
			setRace(Race::HUMAN);
			break;
		case DWARF:
			setStr(str + DwarfRace::STR_ADJ);
			setDex(dex + DwarfRace::DEX_ADJ);
			setCon(con + DwarfRace::CON_ADJ);
			setIte(ite + DwarfRace::ITE_ADJ);
			setWis(wis + DwarfRace::WIS_ADJ);
			setCha(cha + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			setRace(Race::DWARF);
			break;
		case ELF:
			setStr(str + ElfRace::STR_ADJ);
			setDex(dex + ElfRace::DEX_ADJ);
			setCon(con + ElfRace::CON_ADJ);
			setIte(ite + ElfRace::ITE_ADJ);
			setWis(wis + ElfRace::WIS_ADJ);
			setCha(cha + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			setRace(Race::ELF);
			break;
		case GNOME:
			setStr(str + GnomeRace::STR_ADJ);
			setDex(dex + GnomeRace::DEX_ADJ);
			setCon(con + GnomeRace::CON_ADJ);
			setIte(ite + GnomeRace::ITE_ADJ);
			setWis(wis + GnomeRace::WIS_ADJ);
			setCha(cha + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			setRace(Race::GNOME);
			break;
	}
}


// #####################################################################################################


Monster::Monster()
{
}
Monster::Monster(int x, int y, int clipX, int clipY, Graphics* monsterGraphics, int monsterType, int level) : Character()
{
			graphics = monsterGraphics;
			clip->x = (clipX)*16;
			clip->y = (clipY)*16;
			clip->w = 16;
			clip->h = 16;

			createMonster(monsterType, level);

			this->x = x;
			this->y = y;
			
}

void Monster::createMonster(int monster, int level)
{//name, level, speed, str, dex, con, ite, wis, cha, damageDice
	switch (monster)
	{
		case SKELETON:
			setAllMonster("Skeleton", level, 30, 13, 13, 0, 0, 10, 1, Dice::D12);
			break;
		case ELF:
			setAllMonster("Elf", level, 30, 13, 13, 10, 10, 9, 8, Dice::D8);
			break;
		case GOBLIN:
			setAllMonster("Goblin", level, 30, 11,13,12,10,9,6, Dice::D8);
			break;
		case LIZARD:
			setAllMonster("Lizard", level, 20, 3,15,10,1,12,2, Dice::D8);
			break;
		case VINE:
			setAllMonster("Assassin Vine", level, 5, 20,10,16,0,13,9, Dice::D16);
			break;
		case MEDUSA:
			setAllMonster("Medusa", level, 30, 10,15,12,12,13,15, Dice::D20);
			break;
	}
}

int Monster::getMonsterHP(int level)
{
	if (level == 1)
		return(10);
	else
		return(10+level*5);
}

void Monster::setAllMonster(char* name, int level, int speed, int str, int dex, int con, int ite, int wis, int cha, int diceType)
{
	setLevel(level);
	setHp(getMonsterHP(level));
	setSpeed(speed);

	setName(name);

	setStr(str);
	setDex(dex);
	setCon(con);
	setIte(ite);
	setWis(wis);
	setCha(cha);

	setDamageDiceType(diceType);

}

void Monster::setDamageDiceType(int diceType) {
	this->damageDiceType = diceType;
}

int Monster::getDamageDiceType() {
	return damageDiceType;
}

void Monster::died()
{
	if(isDead())
	{
		Character::died();
		clip->x += TILE_SIZE;
	}
}


// #####################################################################################################

Graphics* Treasure::treasureGraphics = NULL;

Treasure::Treasure()
{
	opened = false;
	if(treasureGraphics == NULL)
		treasureGraphics = new Graphics(".\\images\\treasure.png",0xFF,0x0,0xFF);
	graphics = treasureGraphics;
	clip = new SDL_Rect();
	clip->x = clip->y = 0;
	clip->w = clip->h = 16;
}

void Treasure::treasureOpened()
{
	opened = true;
	clip->x = 16;
}

bool Treasure::isOpen()
{
	return opened;
}

// #####################################################################################################

MoneyTreasure::MoneyTreasure(int awardInGold) : Treasure()
{
	if(awardInGold > 0)
		rewardMoney.addGold(awardInGold);
}

void MoneyTreasure::obtainTreasure(ControllableCharacter *recepient)
{
	if(!opened)
	{
		recepient->money += rewardMoney;
		std::cout << "You found " << rewardMoney.getGold() << " gold in the treasure!\n";
		treasureOpened();
	}
}

// #####################################################################################################

ItemTreasure::ItemTreasure(Item* newRewardItem) : Treasure()
{
	rewardItem = newRewardItem;
}


void ItemTreasure::obtainTreasure(ControllableCharacter *recepient)
{
	if(!opened)
	{
		recepient->addItem(rewardItem);
		std::cout << "You found the item '" << rewardItem->name << "' in the treasure!\n";
		treasureOpened();
	}
}

// #####################################################################################################

