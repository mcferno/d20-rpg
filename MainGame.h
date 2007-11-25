#ifndef MAINGAME_H
	#define MAINGAME_H

#include "Screens.h"
#include "ShopScreen.h"
#include "EquipScreen.h"
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
	const char *graphics;

	// the transparent color to mask out of the graphics image (RGB, in hex)
	int alphaR, alphaG, alphaB;

	// relative path to the map index on disk
	const char *index;
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


	SDL_Surface *background;

	SDL_Surface *walkableHighlight, *targetableHighlight;

	SDL_Surface *msgInfo[2];
	SDL_Surface *msgCustomInfo[2];

	Button controlBtns[4];
	Button mapBtns[4];
	Rect exitBtn;

	Character *currentPlayer;
	int currSpeed;
	bool attackedThisRound;
	int numPlayers;

	bool subScreenSignal;

	// queue used to track the players and their relative turns
	queue<Character*> playOrder;

	// an array of Monsters for the current level
	Monster *enemies, *selectedEnemy;
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


	static const int CHARACTER_PANEL_X = 2*TILE_SIZE;
	static const int CHARACTER_PANEL_Y = 32*TILE_SIZE;

	static const int INFO_PANEL_X = 41*TILE_SIZE;
	static const int INFO_PANEL_Y = 2*TILE_SIZE;

	void loadLevel();
	void doInitiativeRoll();
	void nextTurn();
	bool isTileOccupied(int,int);
	bool isTileWalkable(int,int);
	int tileToPixelsX(int);
	int tileToPixelsY(int);

	void characterMoved();
	bool inRange(Object*,int);
	bool clickedEnemy(int,int);
	void selectEnemy(Monster*);
	void unselectEnemy();

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

	void showEquipScreen();
	void openTreasure();

	void paintNow();
	void paintObject(Object*);
	void paintRange(int,SDL_Surface*,bool=false);
	void paintInfoPanel();
	void paintCharacterPanel();

	//fight logic
	bool damageRoll;
	int playerInitiativeRoll, monsterInitiativeRoll;
	int playerAttackRoll, monsterAttackRoll;
	int playerDamageRoll, monsterDamageRoll;

	int rollDamageMelee();
	int rollDamageRanged();
	int rollDamageMelee(Monster *thisMonster);
	bool fPlayerAttackRoll(Monster *thisMonster);
	bool fMonsterAttackRoll(Monster *thisMonster);
	void fight(Monster *thisMonster);

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

	// used to move game logic independently of key/mouse events
	void tick();
};

#endif
