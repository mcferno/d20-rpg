#include "Graphics.h"
#include "Sprites.h"
#include <queue>

using std::queue;

extern Graphics *selectScreen;
extern Graphics *selectionScreenBackground;
extern Graphics *characterSprites;
extern Character *mainCharacter;

class SelectionScreen
{
private:
	// pointer to the main screen which will be painted to
	SDL_Surface *screen;
	static const int NUM_CHARACTERS = 5;
	static const int NUM_RACES = 4;
	static const int NUM_CLASSES = 1;
	GraphicsSelection availableSprites[NUM_CHARACTERS];
	GraphicsSelection availableRaces[NUM_RACES];
	GraphicsSelection availableClasses[NUM_CLASSES];
	int selectedSprite; 
	int selectedRace;
	int selectedClass;

	bool *signalCompletion;

	void init();
	void paintGraphicsSelection(GraphicsSelection &);

public:
	SelectionScreen();
	SelectionScreen(SDL_Surface *);
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void paint();
	void setSignal(bool *);
};

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



class MainGame
{
private:
	// pointer to the main screen which will be painted to
	SDL_Surface *screen;

	Map gameMap;
	Level *level;
	Character *currentPlayer;
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

	void loadLevel();
	void paintNow();
	void paintObject(Object*);
	void doInitiativeRoll();
	void nextTurn();
	void doAITurn();

public:
	MainGame();
	MainGame(SDL_Surface *);
	void paint();
	void init();
	void keyUp();
	void keyDown();
	void keyLeft();
	void keyRight();
};