/********************************************************
Author: Patrick McFern & Chris Karpyszyn

All classes related to screens (ex: Character selection
  screen, Main game screen) with the logic to run them
  and properly interact with the user.

*********************************************************/

#ifndef SCREENS_H
	#define SCREENS_H

#include "Graphics.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Sprites.h"
#include "Items.h"
#include "Inventory.h"

// inherited x,y,w,h are in px
class Screen : public Rect
{
protected:
	static SDL_Surface *screen;
	static TTF_Font *fontCalibri;
	static TTF_Font *fontCalibriBold;
	static TTF_Font *fontCalibriTiny;
	static SDL_Surface *highlightTile;

	// the chosen character, shared among the selection screen and main game
	static ControllableCharacter *mainCharacter;
public:
	virtual void paint() = 0;

	Screen(int,int,int,int);

	static void init(SDL_Surface *);
};

#endif