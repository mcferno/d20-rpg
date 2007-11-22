#ifndef SELECTION_SCREEN_H
	#define SELECTION_SCREENS_H


#include <iostream>
#include <string>
#include <sstream>
#include "Screens.h"
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
	Graphics *characterSprites;
	SDL_Surface *selectScreen;

	//color of text
	SDL_Color textColorWhite;
	SDL_Color textColorBlack;

	// variables needed to offer the user a set of choices for their character
	static const int NUM_CHARACTERS = 5;
	static const int NUM_RACES = 4;
	static const int NUM_CLASSES = 1;
	GraphicsSelection availableSprites[NUM_CHARACTERS];
	GraphicsSelection availableRaces[NUM_RACES];
	GraphicsSelection availableClasses[NUM_CLASSES];

	//which is selected, -1 means nothing selected
	int selectedSprite; 
	int selectedRace;
	int selectedClass;

	//temp holders for each roll/mod when clicking roll button
	int rollStr, rollDex, rollCon, rollIte, rollWis, rollCha;
	int modStr, modDex, modCon, modIte, modWis, modCha;
	//temp holders for the string representation to print to screen
	char sStr[4], sDex[4], sCon[4], sIte[4], sWis[4], sCha[4]; //roll
	char smStr[4], smDex[4], smCon[4], smIte[4], smWis[4], smCha[4]; //mod roll
	char sadjStr[4], sadjDex[4], sadjCon[4], sadjIte[4], sadjWis[4], sadjCha[4];

	//if youve clicked the roll button or not
	bool hasRolled;

	bool inBounds(GraphicsSelection &, int, int);



	// whether or not the main character has been created
	bool *signalCompletion;

	void paintGraphicsSelection(GraphicsSelection &);

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

	//converts a given int to a string
	string int2string(const int);

	int abs(int);

public:

	playerClass myClass;
	race myRace;


	SelectionScreen(int, int, int, int);

	void init();
	void paint();

	// a 'semaphore' of sorts used to synchonize with the class which instanciated it
	void setSignal(bool *);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};


#endif