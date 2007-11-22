#ifndef START_SCREENS_H
	#define START_SCREENS_H

#include "Screens.h"

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

	bool *signalCompletionS;

	void init();

	bool inBounds(GraphicsSelection &, int, int);
	bool musicStarted;

	

public:

	StartScreen(int, int, int, int);
	void paint();

	// a 'semaphore' of sorts used to synchonize with the class which instanciated it
	void setSignalS(bool *);

	// handles mouse interactions
	void mouseLeft(int,int);
	void mouseRight(int,int);
};

#endif