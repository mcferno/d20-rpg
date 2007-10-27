#include "Graphics.h"
#include "Sprites.h"

extern Graphics *characterSprites;
extern Character *mainCharacter;

class SelectionScreen
{
private:
	SDL_Surface *screen;

	void init();
public:
	SelectionScreen(SDL_Surface *);
	void mouseLeft(int,int);
	void mouseRight(int,int);

	void paint();
};



class MainGame
{
	SDL_Surface *screen;
public:
	MainGame(SDL_Surface *);
};