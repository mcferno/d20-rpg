/********************************************************
Author: Christopher Karpyszyn
Created: Oct 8th 2007
Version 1.0

This is the Base Object class
Which contains information about anything that will be 
drawn to the map.

A child to the Object class is the Character class which
automatically creates a player when it is called.
*********************************************************/

#ifndef SPRITES_H
	#define SPRITES_H

#include "Dice.h"
#include "Graphics.h"
#include "Money.h"
#include "Inventory.h"
#include <iostream>

//These are the types of races allowed in the game
//As of October 10th only HUMAN and DWARF have been implemented
enum race {HUMAN, DWARF, ELF, GNOME};
enum playerClass {FIGHTER, RANGER};

// #####################################################################################################

//Class Object will store information about any object drawn on the map
class Object
{
public:
	// what tile is this object on.
	// In tiles (regardless of the map offset)
	int x, y;

	// which graphics to look at in order to paint?
	Graphics* graphics;

	// x, y, w, h ; which graphic to paint from a tileset.
	SDL_Rect* clip;

	// paints the object, depending on what type it is..
	//virtual paint();

	// default constructor
	Object();
};

// #####################################################################################################

// any object that doesn't move, but occupies space on the map
class InatemateObject : public Object
{};

// #####################################################################################################

// An object that moves.
// Character defines the basic rules and attributes of Characters in the game world
class Character : public Object
{
protected:
	// abilities common to all characters: strength, dexterity, consitution, wisdom, charisma
	int str, dex, con, ite, wis, cha;
	char* name;
	
	// hit points, current level, speed (in tiles)
	int hp, level, speed;

	int imyRace, imyClass;

	static const int FIGHTER = 0;
	static const int RANGER = 1;
	virtual void died();

public:

	static const int HAND_COMBAT_DICE = Dice::D3;

	void setRace(int);
	void setClass(int);
	int getRace();
	int getClass();

	char* getName();
	void setName(char*);

	static int getModifier(int);

	static int getAbilityRoll();

	Character();

	// get the sum of a number of rolls, discarding the lowest

	//mutators


	void setStr(int);
	void setDex(int);
	void setCon(int);
	void setIte(int);
	void setWis(int);
	void setCha(int);
	void setAllModifiers(int, int, int, int, int, int);
	void setHp(int);
	void setLevel(int);
	void setSpeed(int);

	//accessors
	int getStr();
	int getDex();
	int getCon();
	int getIte();
	int getWis();
	int getCha();
	int getHp();
	int getLevel();
	virtual int getAC();
	virtual int rollWeaponDamage(int);
	int getSpeed();
	bool isDead();

	// accessors for modifiers
	int getStrMod();
	int getDexMod();
	int getConMod();
	int getIteMod();
	int getWisMod();
	int getChaMod();

	//output
	void showCharacter();

	int getInitiativeRoll();
	int getAttackBonus();
	int getBaseAttackBonus();

};
// #####################################################################################################

class ControllableCharacter : public Character
{
protected:
	// how much money a character starts with
	// currently: 4d4 of gold
	//static const int STARTING_MONEY_DICE_TYPE = Dice::D4;
	static const int STARTING_MONEY_DICE_TYPE = Dice::D20;
	//static const int STARTING_MONEY_DICE_ROLLS = 4;
	static const int STARTING_MONEY_DICE_ROLLS = 15;
	static const int STARTING_MONEY_TYPE = Money::GOLD;

	// how to determine a randomly rolled character ability
	static const int ABILITY_ROLL_DICE_TYPE = Dice::D6;
	static const int ABILITY_ROLL_DICE_ROLLS = 4;

	void rollStartingMoney();

	int numArrows, numBolts, numPotions;
public:

	Money money;
	Inventory inventory;
	Weapon *equippedWeapon;
	Armor *equippedShield, *equippedHelmet, *equippedVest;

	ControllableCharacter();
	void addItem(Item*);
	void removeItem(Item*);
	bool isEquipped(Item*);
	void unEquip(Item*);
	static int getAbilityRoll();
	void setNumArrows(int);
	void setNumBolts(int);
	void setNumPotions(int);
	int getWeaponRange();
	int getNumArrows();
	int getNumBolts();
	int getNumPotions();
	int getAC();
	int rollWeaponDamage();


};

// #####################################################################################################

// a movable object which is controlled by the computer
// to be implemented later.
class Monster : public Character
{
private:

	int damageDiceType;

protected:
	void died();

public:

	static const int SKELETON = 0;
	static const int ELF = 1;
	static const int GOBLIN = 2;
	static const int LIZARD = 3;
	static const int VINE = 4;
	static const int MEDUSA = 5;

	Monster();
	Monster(int, int, int, int, Graphics*, int, int);

	//monster HP depends on level of monster
	int getMonsterHP(int);
	//first int is for type, second int is for level
	void createMonster(int, int);
	//name, level, speed, str, dex, con, ite, wis, cha, damageDice
	void setAllMonster(char*, int, int, int, int, int, int, int, int, int);

	int getDamageDiceType();
	void setDamageDiceType(int);
};

// #####################################################################################################

// Fighter class is the definition of a certain type of character in the game
// When calling the class, it receives a specific race type specified in the above race enumeration
class Fighter : public ControllableCharacter {
public:
	const static int FIGHTER_HP = 10;
	Fighter();
	//does the roll by default, with a given race
	Fighter(race);
	//takes the values to be added for the rolls.
	Fighter(race, int, int, int, int, int, int);
};

class Ranger : public ControllableCharacter {
public:
	const static int RANGER_HP = 10;
	Ranger();
	//does the roll by default, with a given race
	Ranger(race);
	//takes the values to be added for the rolls.
	Ranger(race, int, int, int, int, int, int);
};

// #####################################################################################################

// Race specific characteristics (as defined by the d20 system)
class Race
{
public:
	static const int STR_ADJ = 0;
	static const int DEX_ADJ = 0;
	static const int CON_ADJ = 0;
	static const int ITE_ADJ = 0;
	static const int WIS_ADJ = 0;
	static const int CHA_ADJ = 0;

	static const int HUMAN = 0;
	static const int DWARF = 1;
	static const int ELF = 2;
	static const int GNOME = 3;
};

// #####################################################################################################

class HumanRace : public Race
{
public:
	static const int SPEED = 30;
};

// #####################################################################################################

class DwarfRace : public Race
{
public:
	static const int DEX_ADJ = -2;
	static const int CON_ADJ = 2;
	static const int SPEED = 20;
};

// #####################################################################################################

class ElfRace : public Race
{
public:
	static const int DEX_ADJ = 2;
	static const int CON_ADJ = -2;
	static const int SPEED = 30;
};

// #####################################################################################################

class GnomeRace : public Race
{
public:
	static const int STR_ADJ = -2;
	static const int CON_ADJ = 2;
	static const int SPEED = 20;
};

// #####################################################################################################

class Treasure : public Object
{
private:
	static Graphics* treasureGraphics;
protected:
	bool opened;
	void treasureOpened();
public:
	Treasure();
	bool isOpen();
	virtual void obtainTreasure(ControllableCharacter*) = 0;
};

// #####################################################################################################

class MoneyTreasure : public Treasure
{
private:
	Money rewardMoney;
public:
	MoneyTreasure(int);
	virtual void obtainTreasure(ControllableCharacter*);
};

// #####################################################################################################

class ItemTreasure : public Treasure
{
private:
	Item* rewardItem;
public:
	ItemTreasure(Item*);
	virtual void obtainTreasure(ControllableCharacter*);
};

// #####################################################################################################


#endif

