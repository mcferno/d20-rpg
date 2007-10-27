#include "Screens.h"
#include <iostream>

Graphics *characterSprites = NULL;
Character *mainCharacter = NULL;

void SelectionScreen::init()
{
	characterSprites = new Graphics(".\\images\\characters.bmp");
}

SelectionScreen::SelectionScreen(SDL_Surface *newScreen)
{
	screen = newScreen;
	init();
}

void SelectionScreen::paint()
{

	// paint background
	// if player graphic selected? paint in selected character area
	// if race selected? 


	applySurface(0,0,characterSprites->image,screen);
}

void SelectionScreen::mouseLeft(int x, int y)
{
}

void SelectionScreen::mouseRight(int x, int y)
{
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


MainGame::MainGame(SDL_Surface *newScreen)
{
	screen = newScreen;
}
