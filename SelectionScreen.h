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
	SDL_Surface *highlightTile;
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

	//temp holders for each roll when clicking roll button
	int rollStr, rollDex, rollCon, rollIte, rollWis, rollCha;

	//if youve clicked the roll button or not
	bool hasRolled;

	bool inBounds(GraphicsSelection &, int, int);

	//enumerated types to show which race/class is selected
	playerClass myClass;
	race myRace;

	// whether or not the main character has been created
	bool *signalCompletion;

	void init();
	void paintGraphicsSelection(GraphicsSelection &);

	//what happens when you click the roll button
	void rollButton();

	//two variables to determine starting point
	void paintMessage(int, int);
	void paintCharacterMessage(int, int);

	//find your race/class depending on which button you select
	race findRace(int);
	playerClass findClass(int);

	//converts a given int to a string
	string int2string(const int);

public:
	SelectionScreen(int, int, int, int);
	void paint();

	// a 'semaphore' of sorts used to synchonize with the class which instanciated it
	void setSignal(bool *);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};


#endif