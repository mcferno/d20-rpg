#include "Sprites.h"

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

int Character::getModifier(int base)
{
	return (base/2)-5;
}

// Armor Class (AC): How hard a character is to hit. 
// 10 + armor bonus + shield bonus + size modifier + dexterity modifier
int Character::getAC(){
	return 10 + getModifier(dex);
}
//END ACCESSORS

const int Fighter::FIGHTER_HP = 10;

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
			//setAC(0);
			setStr(myDice.characterRoll());
			setDex(myDice.characterRoll());
			setCon(myDice.characterRoll());
			setIte(myDice.characterRoll());
			setWis(myDice.characterRoll());
			setCha(myDice.characterRoll());
			break;
		case DWARF:
			//setAC(0);
			setStr(myDice.characterRoll());
			setDex(myDice.characterRoll());
			setCon(myDice.characterRoll() + 2);
			setIte(myDice.characterRoll());
			setWis(myDice.characterRoll());
			setCha(myDice.characterRoll() - 2);
			break;
	}
}




