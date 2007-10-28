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
enum race {HUMAN, DWARF, ELF, GNOME, HALF_ELF, HALF_ORC, HALFLING};





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
};


class InatemateObject : public Object
{};


//Character is a child of Object
//Character defines the basic rules and attributes of Characters in the game world
class Character : public Object
{
private:
	// abilities common to all characters: strength, dexterity, consitution, wisdom, charisma
	int str, dex, con, ite, wis, cha;
	
	// hit points, current level
	int hp, level;

	int getModifier(int);
	
public:
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
	//void setAC(int);

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

	// accessors for modifiers
	int getStrMod();
	int getDexMod();
	int getConMod();
	int getIteMod();
	int getWisMod();
	int getChaMod();
};

// --------------------------




// see player, might be removed.
// basic AI could go here..
class Monster : public Character
{
};



//Class fighter is a child of Character
//Fighter class is the definition of a certain type of character in the game
//When calling the class, it receives a specific race type specified in the above race enumeration
class Fighter : public Character {
public:
	const static int FIGHTER_HP;
	Fighter();
	Fighter(race);
};


#endif

