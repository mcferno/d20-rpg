#ifndef START_SCREENS_H
	#define START_SCREENS_H

#include "Screens.h"
#include "MapEditor.h"

class StartScreen : public Screen
{
private:

	Mix_Music *startMusic;

	SDL_Surface *startScreen0;
	SDL_Surface *startScreen1;
	SDL_Surface *startScreen2;
	SDL_Surface *startScreen3;
	SDL_Surface *startScreen4;
	SDL_Surface *startScreen5;
	SDL_Surface *startScreen6;

	MapEditor *mapEditor;

	void init();

	bool inBounds(GraphicsSelection &, int, int);
	bool musicStarted;

	int state;
	bool isDone;

	static const int STATE_SHOW_SCREEN = 0;
	static const int STATE_MAP_EDITOR = 1;

public:

	StartScreen(int, int, int, int);
	void paint();
	void showMapEditor(int=1);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};

#endif