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

#ifndef OBJECT_H
	#define OBJECT_H

#include "dice.h"
#include <iostream>

//These are the types of races allowed in the game
//As of October 10th only HUMAN and DWARF have been implemented
enum race {HUMAN, DWARF, ELF, GNOME, HALF_ELF, HALF_ORC, HALFLING};

//Class Object will store information about any object drawn on the map
class Object
{
	//THIS IS A STUB
	//FOR LATER IMPLEMENTATION
	int x, y;
};

//Character is a child of Object
//Character defines the basic rules and attributes of Characters in the game world
class Character : public Object
{
	//Modifiers common to all Characters
	//Strenght, Dexterity, Constitution, Intelligence, Wisdom, Charisma 
	//Hitpoints, Level, Armour Class 
private:
	int str, dex, con, ite, wis, cha, hp, level, ac;
	
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
	void setAC(int);
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
};

//Class fighter is a child of Character
//Fighter class is the definition of a certain type of character in the game
//When calling the class, it receives a specific race type specified in the above race enumeration
class Fighter : public Character {
public:
	Fighter(race);
};

//Accessors
extern void setStr(int);
extern void setDex(int);
extern void setCon(int);
extern void setIte(int);
extern void setWis(int);
extern void setCha(int);
extern void setHp(int);
extern void setLevel(int);
extern void setAC(int);

//allows to set all modifiers at once
//in order: str, dex, con, ite, wis, cha
extern void setAllModifiers(int, int, int, int, int, int);

//Mutators
extern int getStr();
extern int getDex();
extern int getCon();
extern int getIte();
extern int getWis();
extern int getCha();
extern int getHp();
extern int getLevel();
extern int getAC();

#endif

