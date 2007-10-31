#include "Sprites.h"

Object::Object()
{
	x = y = -1;
	clip = new SDL_Rect();
	graphics = NULL;
}

Character::Character() : Object()
{
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

/*
void Character::setAC(int ac){
	if (ac <= 0)
		this->ac = 0;
	else
		this->ac = ac;
}
*/
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
	std::cout << "STR: " << getStr() << "\n";
	std::cout << "DEX: " << getDex() << "\n";
	std::cout << "CON: " << getCon() << "\n";
	std::cout << "ITE: " << getIte() << "\n";
	std::cout << "WIS: " << getWis() << "\n";
	std::cout << "CHA: " << getCha() << "\n";
	std::cout << "SPEED: " << getSpeed() << "\n";
	std::cout << "HP: " << getHp() << "\n";
}

Fighter::Fighter()
{

}

//Constructor class
//Takes a type race to specify which race the character is
//Each race may have specific additions to the modifiers
//Hitpoints, Level, and AC begin at a predetermined base
//Str, Dex, Con, Ite, Wis, Cha are all determined randomly by the character roll.
Fighter::Fighter(race myrace) {

	setHp(FIGHTER_HP);
	setLevel(1);
	Dice myDice;

	switch (myrace)
	{
		case HUMAN:
			setStr(myDice.characterRoll() + HumanRace::STR_ADJ);
			setDex(myDice.characterRoll() + HumanRace::DEX_ADJ);
			setCon(myDice.characterRoll() + HumanRace::CON_ADJ);
			setIte(myDice.characterRoll() + HumanRace::ITE_ADJ);
			setWis(myDice.characterRoll() + HumanRace::WIS_ADJ);
			setCha(myDice.characterRoll() + HumanRace::CHA_ADJ);
			setSpeed(HumanRace::SPEED);
			showCharacter();
			break;
		case DWARF:
			setStr(myDice.characterRoll() + DwarfRace::STR_ADJ);
			setDex(myDice.characterRoll() + DwarfRace::DEX_ADJ);
			setCon(myDice.characterRoll() + DwarfRace::CON_ADJ);
			setIte(myDice.characterRoll() + DwarfRace::ITE_ADJ);
			setWis(myDice.characterRoll() + DwarfRace::WIS_ADJ);
			setCha(myDice.characterRoll() + DwarfRace::CHA_ADJ);
			setSpeed(DwarfRace::SPEED);
			showCharacter();
			break;
		case ELF:
			setStr(myDice.characterRoll() + ElfRace::STR_ADJ);
			setDex(myDice.characterRoll() + ElfRace::DEX_ADJ);
			setCon(myDice.characterRoll() + ElfRace::CON_ADJ);
			setIte(myDice.characterRoll() + ElfRace::ITE_ADJ);
			setWis(myDice.characterRoll() + ElfRace::WIS_ADJ);
			setCha(myDice.characterRoll() + ElfRace::CHA_ADJ);
			setSpeed(ElfRace::SPEED);
			showCharacter();
			break;
		case GNOME:
			setStr(myDice.characterRoll() + GnomeRace::STR_ADJ);
			setDex(myDice.characterRoll() + GnomeRace::DEX_ADJ);
			setCon(myDice.characterRoll() + GnomeRace::CON_ADJ);
			setIte(myDice.characterRoll() + GnomeRace::ITE_ADJ);
			setWis(myDice.characterRoll() + GnomeRace::WIS_ADJ);
			setCha(myDice.characterRoll() + GnomeRace::CHA_ADJ);
			setSpeed(GnomeRace::SPEED);
			showCharacter();
			break;
	}
}

Monster::Monster() : Character()
{
}




