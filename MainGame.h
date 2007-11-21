#ifndef MAINGAME_H
	#define MAINGAME_H

#include "Screens.h"
#include "ShopScreen.h"
#include "EquipScreen.h"
#include "FightScreen.h"
#include <queue>
using std::queue;

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
	EquipScreen *equipScreen;
	FightScreen *fightScreen;

	Character *currentPlayer;
	int currSpeed;
	int numPlayers;

	bool subScreenSignal;

	// queue used to track the players and their relative turns
	queue<Character*> playOrder;

	// an array of Monsters for the current level
	Monster *enemies;
	int numEnemies;

	// an array of treasure found scattered on the map
	Treasure** treasure;
	int numTreasure;

	int shopDoorX, shopDoorY, exitDoorX, exitDoorY;

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
	static const int STATE_EQUIP = 4;
	static const int STATE_FIGHT = 5;
	void loadLevel();
	void paintNow();
	void paintObject(Object*);
	void doInitiativeRoll();
	void nextTurn();
	bool isTileOccupied(int,int);
	bool isTileWalkable(int,int);

	Uint32 startAITurn;
	// time in milliseconds to wait between AI actions, obeyed when possible
	// actual turns will occur in the time range [ AI_TURN_TIME, AI_TURN_TIME+FRAME_RATE_SLEEP [
	static const Uint32 AI_TURN_TIME = 200; 

	// does simple AI for enemies of the level
	void doAITurn();

	// enters the shop if you are standing in its doorway
	void enterShop();

	// exits the level if you are in the exit door of the level
	void exitLevel();

public:
	MainGame(int, int, int, int);
	void init();
	void paint();

	// keyboard interaction, allowing the human player to control his character
	void keyUp();
	void keyDown();
	void keyLeft();
	void keyRight();

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);

	void showShop();
	void showEquipScreen();
	void showFightScreen();
	void openTreasure();

	// used to move game logic independently of key/mouse events
	void tick();
};

#endif
