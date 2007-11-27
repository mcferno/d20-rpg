#include "Screen.h"

// initialize the static pointers
ControllableCharacter* Screen::mainCharacter = NULL;
SDL_Surface* Screen::screen = NULL;
TTF_Font* Screen::fontCalibri = NULL;
TTF_Font* Screen::fontCalibriBold = NULL;
TTF_Font* Screen::fontCalibriTiny = NULL;
SDL_Surface* Screen::highlightTile = NULL;
SDL_Color Screen::colorWhite = SDL_Color();
SDL_Color Screen::colorBlack = SDL_Color();

Screen::Screen(int newX, int newY, int newW, int newH)
{
	// set the size of the window
	x = newX;
	y = newY;
	w = newW;
	h = newH;

	// no signal has been set
	signal = NULL;
}

Screen::~Screen()
{
	std::cout << "Destroying Screen\n";
}

void Screen::init(SDL_Surface *newScreen)
{
	Screen::screen = newScreen;
	Screen::highlightTile = loadImage(".\\images\\highlight.png",0x0,0xFF,0xFF);
	Screen::fontCalibri = TTF_OpenFont( ".\\fonts\\calibri.ttf", 16 );
	Screen::fontCalibriTiny = TTF_OpenFont ( ".\\fonts\\calibri.ttf", 12 );
	Screen::fontCalibriBold = TTF_OpenFont ( ".\\fonts\\calibrib.ttf", 16 );
	colorWhite.r = colorWhite.g = colorWhite.b = 0xFF;
	colorBlack.r = colorBlack.g = colorBlack.b = 0x0;
}

void Screen::setSignal(bool *newSignal)
{
	if(newSignal != NULL)
		signal = newSignal;
}

void Screen::signalCompletion()
{
	if(signal != NULL)
		*signal = true;
}


