#ifndef MAINGAME_H
	#define MAINGAME_H

#include "Screen.h"
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
	SDL_Surface *background;
	SDL_Surface *winScreen;
	SDL_Surface *loseScreen;

	// the map which displays the level
	Map gameMap;

	// the information needed to load a level
	Level *level;

	// the screen the user will see in order to buy and sell items
	ShopScreen *shopScreen;

	// the screen the user will see to equip items from his/her inventory
	EquipScreen *equipScreen;

	// colored tiles used to indicate walkable tiles and shooting range
	SDL_Surface *walkableHighlight, *targetableHighlight;

	// rendered informative text
	SDL_Surface *msgInfo[2];
	SDL_Surface *msgCustomInfo[2];
	SDL_Surface *msgFixedInfo[7];
	SDL_Surface *msgVariableInfo[10];

	// buttons used to interact with the game
	Button controlBtns[4];
	Button mapBtns[4];
	Rect exitBtn;

	// the player who is currently playing his/her turn
	Character *currentPlayer;

	// the number of steps the current player has left to take
	int currSpeed;

	// whether or not the human player has attacked
	bool attackedThisRound;

	// total number of players 
	int numPlayers;

	// a 'signal' used to allow any sub screen to indicate to this class that it wants to close
	bool subScreenSignal;

	// queue used to track the players and their relative turns
	queue<Character*> playOrder;

	// an array of Monsters for the current level
	Monster *enemies, *selectedEnemy;
	int numEnemies;

	// an array of treasure found scattered on the map
	Treasure** treasure;
	int numTreasure;

	// the shop and exit door entry points on the map
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

	// the location of the informative panels
	static const int CHARACTER_PANEL_X = 2*TILE_SIZE;
	static const int CHARACTER_PANEL_Y = 32*TILE_SIZE;
	static const int INFO_PANEL_X = 41*TILE_SIZE;
	static const int INFO_PANEL_Y = 2*TILE_SIZE;

	// the time when the AI did its last move
	Uint32 startAITurn;

	// time in milliseconds to wait between AI actions, obeyed when possible
	// actual turns will occur in the time range [ AI_TURN_TIME, AI_TURN_TIME+FRAME_RATE_SLEEP [
	static const Uint32 AI_TURN_TIME = 200; 

	// variables used to initiate battle
	bool damageRoll;
	int playerInitiativeRoll, monsterInitiativeRoll;
	int playerAttackRoll, monsterAttackRoll;
	int playerDamageRoll, monsterDamageRoll;

	// loads a level into the GameMap
	void loadLevel();

	// decides which of the many players will go in what order
	void doInitiativeRoll();

	// indicates that the current user has ended his/her turn, find the next player to go
	void nextTurn();

	// whether or not there is a blocking object currently located on a tile
	bool isTileOccupied(int,int);

	// whether or not a tile can be walked on (if not, it can never be occupied)
	bool isTileWalkable(int,int);

	// converts map coordinates to screen pixels (for mouse clicks)
	int tileToPixelsX(int);
	int tileToPixelsY(int);

	// decrement the user's 'steps left'
	void characterMoved();

	// decides whether an object is within a given range
	bool inRange(Object*,int);

	// verifies if a mouse click corresponds to an enemy
	bool clickedEnemy(int,int);

	// renders the enemy specific text to inform the user
	void selectEnemy(Monster*);
	void unselectEnemy();

	// does simple AI for enemies of the level
	void doAITurn();

	// enters the shop if you are standing in its doorway
	void enterShop();

	// exits the level if you are in the exit door of the level
	void exitLevel();

	// bring the equipping screen up
	void showEquipScreen();

	// open any adjacent treasure chests
	void openTreasure();

	// paints and updates as quickly as possible (important for AI movement)
	void paintNow();
	void paintObject(Object*);
	void paintInfoPanel();
	void paintCharacterPanel();
	void refreshVariableInfo();
	void setFixedInfo();

	// uses a shortest path algorithm to show all tiles which can be reached, and paints them
	// based on the following: the tile where you currently stand takes 0 units to reach, any
	// directly adjacent tile (up,down,left,right) takes +1 units to reach, any tile directly
	// adjacent to that one takes +2 units to reach and so on. finally, paint any tile that has
	// a unit value <= to the range you want to paint.
	void paintRange(int,SDL_Surface*,bool=false);

	// carries out in-game fighting
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

	// allows the shop to be invoked (like by keypress)
	void showShop();

	// used to move game logic independently of key/mouse events
	void tick();
};

#endif
