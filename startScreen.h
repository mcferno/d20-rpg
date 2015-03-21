#ifndef START_SCREENS_H
	#define START_SCREENS_H

#include "Screen.h"
#include "MapEditor.h"

/*
 * Class StartScreen: Loads up a start graphics. Loads music into the game. 
	Allows the user to choose between starting the game or editing maps.
	When clicking start game, Startscreen transitions to selectScreen
 */

class StartScreen : public Screen
{
private:

	Mix_Music *startMusic;

	SDL_Surface *background;
	//The buttons
	Rect btnStartGame, btnEditLevel1, btnEditLevel2, btnCreateNewLevel;

	MapEditor *mapEditor;

	void init();
	bool musicStarted;

	int state;
	bool isDone;

	static const int STATE_SHOW_SCREEN = 0;
	static const int STATE_MAP_EDITOR = 1;

public:

	StartScreen(int, int, int, int);
	~StartScreen();
	void paint();
	void showMapEditor(int = 0);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);

	void keyW();
};

#endif
