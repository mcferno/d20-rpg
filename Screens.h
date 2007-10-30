#include "Graphics.h"
#include "Sprites.h"


extern Graphics *selectScreen;
extern Graphics *characterSprites;
extern Character *mainCharacter;

class SpriteSelection
{
public:
	// on screen location
	int x,y;
	// clip relating to a sprite sheet
	SDL_Rect clip;
};

class RaceSelection
{
public:
	int x,y;
	SDL_Rect clip;
};

class ClassSelection
{
public:
	// on screen location
	int x,y;
	SDL_Rect clip;
};

class SelectionScreen
{
private:
	SDL_Surface *screen;
	static const int NUM_CHARACTERS = 5;
	static const int NUM_RACES = 4;
	static const int NUM_CLASSES = 1;
	SpriteSelection availableSprites[NUM_CHARACTERS];
	RaceSelection availableRaces[NUM_RACES];
	ClassSelection availableClasses[NUM_CLASSES];
	int selectedSprite; 
	int selectedRace;
	int selectedClass;

	void init();
	void paintSpriteSelection(SpriteSelection);
public:
	SelectionScreen();
	SelectionScreen(SDL_Surface *);
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void paint();
};


class Level
{
public:
	char *graphics;
	char *index;
	int alphaR;
	int alphaG;
	int alphaB;
};



class MainGame
{
private:
	SDL_Surface *screen;
	int currentLevel;
	int state;
	Map gameMap;
	Level *level;

	static const int LEVEL_1 = 1;

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