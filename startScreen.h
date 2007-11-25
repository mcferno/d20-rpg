#ifndef START_SCREENS_H
	#define START_SCREENS_H

#include "Screens.h"
#include "MapEditor.h"

class StartScreen : public Screen
{
private:

	Mix_Music *startMusic;

	SDL_Surface *background;
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
