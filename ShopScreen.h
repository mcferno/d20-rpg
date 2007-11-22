#ifndef SHOPSCREEN_H
	#define SHOPSCREEN_H

#include "Screens.h"

class ShopScreen : public Screen
{
private:
	SDL_Surface *background;
	SDL_Surface *buttons;

	SDL_Surface *msgItem;
	SDL_Surface *msgName;
	SDL_Surface *msgPrice;
	SDL_Surface *msgGold;
	SDL_Surface *msgNoMoney;
	SDL_Surface *msgCurrItemName;
	SDL_Surface *msgCurrItemPrice;
	SDL_Surface *msgQuantity;
	SDL_Surface *msgItemQuantity;
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
	bool clickedExit(int,int);
	void selectedItem(Item*,bool);
	void deselectItems();
	void paintItem(int, int, Item*);
public:
	ShopScreen(int, int, int, int);
	void paint();
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void userExited();
};

#endif