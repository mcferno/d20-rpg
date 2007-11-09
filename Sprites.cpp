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
	rollStartingMoney();
}

void Character::rollStartingMoney()
{
	money.addCoin(Dice::roll(STARTING_MONEY_DICE_TYPE,STARTING_MONEY_DICE_ROLLS),STARTING_MONEY_TYPE);
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
		this->hp = 0;
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
	return 10 + getModifier(dex);
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
		<< "HP: " << getHp() << "\n"
		<< "Starting Money: " << money.getGold() << " gold.\n";
}

int Character::getAbilityRoll()
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

// #####################################################################################################

Fighter::Fighter()
{

}

//Constructor class
//Takes a type race to specify which race the character is
//Each race may have specific additions to the modifiers
//Hitpoints, Level, and AC begin at a predetermined base
//Str, Dex, Con, Ite, Wis, Cha are all determined randomly by the character roll.
Fighter::Fighter(race myrace) : Character() {

	setHp(FIGHTER_HP);
	setLevel(1);

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
			showCharacter();
			break;
		case DWARF:
			setStr(getAbilityRoll() + DwarfRace::STR_ADJ);
			setDex(getAbilityRoll() + DwarfRace::DEX_ADJ);
			setCon(getAbilityRoll() + DwarfRace::CON_ADJ);
			setIte(getAbilityRoll() + DwarfRace::ITE_ADJ);
			setWis(getAbilityRoll() + DwarfRace::WIS_ADJ);
			setCha(getAbilityRoll() + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			showCharacter();
			break;
		case ELF:
			setStr(getAbilityRoll() + ElfRace::STR_ADJ);
			setDex(getAbilityRoll() + ElfRace::DEX_ADJ);
			setCon(getAbilityRoll() + ElfRace::CON_ADJ);
			setIte(getAbilityRoll() + ElfRace::ITE_ADJ);
			setWis(getAbilityRoll() + ElfRace::WIS_ADJ);
			setCha(getAbilityRoll() + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			showCharacter();
			break;
		case GNOME:
			setStr(getAbilityRoll() + GnomeRace::STR_ADJ);
			setDex(getAbilityRoll() + GnomeRace::DEX_ADJ);
			setCon(getAbilityRoll() + GnomeRace::CON_ADJ);
			setIte(getAbilityRoll() + GnomeRace::ITE_ADJ);
			setWis(getAbilityRoll() + GnomeRace::WIS_ADJ);
			setCha(getAbilityRoll() + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			showCharacter();
			break;
	}
}

// #####################################################################################################

// to be implemented..
Monster::Monster() : Character()
{
}

// #####################################################################################################




