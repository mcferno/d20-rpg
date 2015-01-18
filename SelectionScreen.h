#ifndef SELECTION_SCREEN_H
	#define SELECTION_SCREEN_H


#include <iostream>
#include <string>
#include <sstream>
#include "Screen.h"
using std::string;

/*
 * Class SelectionScreen: Allows the user to visually chose his/her character
 *   to use throughout the game.
 */


class SelectionScreen : public Screen
{
private:
	//text surfaces
	static SDL_Surface *message;
	static SDL_Surface *attributes;
	static SDL_Surface *characterMessage;

	//graphics to be loaded
	SDL_Surface *characterSprites;
	SDL_Surface *selectScreen;

	// variables needed to offer the user a set of choices for their character
	static const int NUM_CHARACTERS = 5;
	static const int NUM_RACES = 4;
	static const int NUM_CLASSES = 2;
	Button availableSprites[NUM_CHARACTERS];
	Rect availableRaces[NUM_RACES];
	Rect availableClasses[NUM_CLASSES];

	//which is selected, -1 means nothing selected
	int selectedSprite; 
	int selectedRace;
	int selectedClass;

	//temp holders for each roll/mod when clicking roll button
	int rollStr, rollDex, rollCon, rollIte, rollWis, rollCha;
	int modStr, modDex, modCon, modIte, modWis, modCha;
	//temp holders for the string representation to print to screen
	std::string sStr, sDex, sCon, sIte, sWis, sCha; //roll
	std::string smStr, smDex, smCon, smIte, smWis, smCha; //mod roll
	std::string sadjStr, sadjDex, sadjCon, sadjIte, sadjWis, sadjCha;

	//if youve clicked the roll button or not
	bool hasRolled;

	//what happens when you click the roll button
	void rollButton();

	//two variables to determine starting point
	void paintMessage(int, int);
	void paintAttributes(int, int);
	//two varibles to determine race, class
	void paintCharacterMessage(int, int);

	//find your race/class depending on which button you select
	race findRace(int);
	playerClass findClass(int);

	//prints out the ADJ for the selected RACE (int selectedRace) and CHARACTERISTC (int)
	void printADJ(int, int, int, int);

	int abs(int);

public:

	//playerClass and race are enumerations
	playerClass myClass;
	race myRace;


	SelectionScreen(int, int, int, int);
	~SelectionScreen();

	void init();
	void paint();

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};


#endif