/********************************************************
Author: Patrick McFern & Chris Karpyszyn

All classes related to screens (ex: Character selection
  screen, Main game screen) with the logic to run them
  and properly interact with the user.

*********************************************************/

#ifndef SCREENS_H
	#define SCREENS_H

#include "Graphics.h"
#include "SDL_ttf.h"
#include "Sprites.h"
#include "Items.h"
#include <queue>
#include <iostream>
using std::queue;

// #####################################################################################################

// inherited x,y,w,h are in px
class Screen : public Rect
{
protected:
	//static SDL_Surface *defaultScreen;
	static SDL_Surface *screen;
	static SDL_Surface *message;
	static TTF_Font *font;

	// the chosen character, shared among the selection screen and main game
	static Character *mainCharacter;
public:
	virtual void paint() = 0;

	Screen(int,int,int,int);

	static void setScreen(SDL_Surface *);
};

// #####################################################################################################

/*
 * Class SelectionScreen: Allows the user to visually chose his/her character
 *   to use throughout the game.
 */
class SelectionScreen : public Screen
{
private:
	//color of text
	SDL_Color textColor;
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
	//if youve clicked the roll button or not
	bool hasRolled;
	Graphics *characterSprites;
	SDL_Surface *highlightTile;
	SDL_Surface *selectScreen;

	// whether or not the main character has been created
	bool *signalCompletion;

	void init();
	void paintGraphicsSelection(GraphicsSelection &);
	bool inBounds(GraphicsSelection &, int, int);

	enum playerClass {FIGHTER};
	playerClass myClass;
	race myRace;

public:
	SelectionScreen(int, int, int, int);
	void paint();

	// a 'semaphore' of sorts used to synchonize with the class which instanciated it
	void setSignal(bool *);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};

// #####################################################################################################

/*
 * Class Level: A simple structure to store information needed to load a level.
 *   A level is basically a set of graphics, and a set of indexes which define
 *   the map for that level.
 */
class Level
{
public:
	// relative path to the graphics file on disk
	char *graphics;

	// the transparent color to mask out of the graphics image (RGB, in hex)
	int alphaR, alphaG, alphaB;

	// relative path to the map index on disk
	char *index;
};

// #####################################################################################################

class ShopScreen : public Screen
{
private:
	// temp
	Uint32 bgColor;

	Weapon *weapons;
	int numWeapons;

	Armor *armor;
	int numArmor;
public:
	ShopScreen(int, int, int, int);
	void paint();
};

// #####################################################################################################

/*
 * Class MainGame: Once the main character has been chosen, the main game class
 *   will take care of implementing the game rules involved in playing. This
 *   includes but is not limited to: determining who will go first, maintaining
 *   proper turns, etc...
 */
class MainGame : public Screen
{
private:
	Map gameMap;
	Level *level;
	ShopScreen *shopScreen;
	Weapon *weapons;

	Character *currentPlayer;
	int currSpeed;
	int numPlayers;

	// queue used to track the players and their relative turns
	queue<Character*> playOrder;

	// an array of Monsters for the current level
	Monster *enemies;
	int numEnemies;

	// simple states tracking the current level
	int currentLevel;
	static const int LEVEL_1 = 1;
	static const int LEVEL_2 = 2;

	// simple states tracking level progress
	int state;
	static const int STATE_LEVEL_START = 0;
	static const int STATE_HUMAN_TURN = 1;
	static const int STATE_AI_TURN = 2;
	static const int STATE_SHOP = 3;
	static const int STATE_BATTLE = 4;

	void loadLevel();
	void paintNow();
	void paintObject(Object*);
	void doInitiativeRoll();
	void nextTurn();

	// does simple AI for enemies of the level
	void doAITurn();

public:
	MainGame(int, int, int, int);
	void init();
	void paint();

	// keyboard interaction, allowing the human player to control his character
	void keyUp();
	void keyDown();
	void keyLeft();
	void keyRight();

	void showShop();
};

// #####################################################################################################

#endif