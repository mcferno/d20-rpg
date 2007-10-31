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
#include <iostream>

//These are the types of races allowed in the game
//As of October 10th only HUMAN and DWARF have been implemented
enum race {HUMAN, DWARF, ELF, GNOME};

// conversion between feet and tile size (5ft per tile)
const int FT_TO_TILES = 5;

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
private:
	// abilities common to all characters: strength, dexterity, consitution, wisdom, charisma
	int str, dex, con, ite, wis, cha;
	
	// hit points, current level, speed (in tiles)
	int hp, level, speed;

	int getModifier(int);
	
public:
	Character();

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
	int getAC();
	int getSpeed();

	// accessors for modifiers
	int getStrMod();
	int getDexMod();
	int getConMod();
	int getIteMod();
	int getWisMod();
	int getChaMod();

	//output
	void showCharacter();
};

// #####################################################################################################

// a movable object which is controlled by the computer
// to be implemented later.
class Monster : public Character
{
public:
	Monster();
};

// #####################################################################################################

// Fighter class is the definition of a certain type of character in the game
// When calling the class, it receives a specific race type specified in the above race enumeration
class Fighter : public Character {
public:
	const static int FIGHTER_HP = 10;
	Fighter();
	Fighter(race);
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

#endif

