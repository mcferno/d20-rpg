#include "Graphics.h"
#include "Sprites.h"


extern Graphics *selectScreen;
extern Graphics *characterSprites;
extern Character *mainCharacter;

class SelectionScreen
{
private:
	SDL_Surface *screen;

	void init();
public:
	//coordinates for the player selection squares
	int cx1, cx2, cx3, cx4, cx5, cy;
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

	static const int LEVEL_1;

	static const int STATE_LEVEL_START;
	static const int STATE_HUMAN_TURN;
	static const int STATE_AI_TURN;

	void loadLevel();
	void paintCharacter(Character*);


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