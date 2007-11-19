/********************************************************
Author: Patrick McFern & Chris Karpyszyn

All classes related to screens (ex: Character selection
  screen, Main game screen) with the logic to run them
  and properly interact with the user.

*********************************************************/

#ifndef SCREENS_H
	#define SCREENS_H

#include "Graphics.h"
//#include "SelectionScreen.h"
#include "SDL_ttf.h"
#include "Sprites.h"
#include "Items.h"
#include "Inventory.h"
#include <queue>
using std::queue;

// #####################################################################################################

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
	static Character *mainCharacter;
public:
	virtual void paint() = 0;

	Screen(int,int,int,int);

	static void init(SDL_Surface *);
};

// #####################################################################################################

/*
 * Class Level: A simple structure to store information needed to load a level.
 *   A level is basically a set of graphics, and a set of indexes which define
 *   the map for that level.
 */
class Level
{
public:
	// relative path to the graphics file on disk
	char *graphics;

	// the transparent color to mask out of the graphics image (RGB, in hex)
	int alphaR, alphaG, alphaB;

	// relative path to the map index on disk
	char *index;
};

// #####################################################################################################

class ShopScreen : public Screen
{
private:
	// temp
	Uint32 bgColor;

	SDL_Surface *background;
	SDL_Surface *buttons;

	SDL_Surface *msgItem;
	SDL_Surface *msgName;
	SDL_Surface *msgPrice;
	SDL_Surface *msgGold;
	SDL_Surface *msgNoMoney;
	SDL_Surface *msgCurrItemName;
	SDL_Surface *msgCurrItemPrice;
	bool showPurchaseError;

	Button buyButton;
	Button sellButton;

	Inventory storeInventory;

	int selectedStoreItem;
	int selectedClientItem;

	SDL_Color fontColorWhite;

	static const int ITEMS_PER_ROW = 13;
	static const int SELL_SECTION_X = 2;
	static const int SELL_SECTION_Y = 5;
	static const int BUY_SECTION_X = 2;
	static const int BUY_SECTION_Y = 12;
	static const int INFO_SECTION_X = 18;
	static const int INFO_SECTION_Y = 5;

	bool clickedButton(int,int,Button);
	void selectedItem(Item*);
	void deselectItems();
	void paintItem(int, int, Item*);
public:
	ShopScreen(int, int, int, int);
	void paint();
	void mouseLeft(int,int);
	void mouseRight(int,int);
};

// #####################################################################################################

#endif