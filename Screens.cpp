#include "Screens.h"

// #####################################################################################################

// initialize the static pointers
Character* Screen::mainCharacter = NULL;
SDL_Surface* Screen::screen = NULL;
TTF_Font* Screen::fontCalibri = NULL;
TTF_Font* Screen::fontCalibriBold = NULL;
TTF_Font* Screen::fontCalibriTiny = NULL;
SDL_Surface* Screen::highlightTile = NULL;

void Screen::init(SDL_Surface *newScreen)
{
	Screen::screen = newScreen;
	Screen::highlightTile = loadImage(".\\images\\highlight.png",0x0,0xFF,0xFF);
	Screen::fontCalibri = TTF_OpenFont( ".\\fonts\\calibri.ttf", 16 );
	Screen::fontCalibriTiny = TTF_OpenFont ( ".\\fonts\\calibri.ttf", 12 );
	Screen::fontCalibriBold = TTF_OpenFont ( ".\\fonts\\calibrib.ttf", 16 );
}

Screen::Screen(int newX, int newY, int newW, int newH)
{
	x = newX;
	y = newY;
	w = newW;
	h = newH;
}

// #####################################################################################################

