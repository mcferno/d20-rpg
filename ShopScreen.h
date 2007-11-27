#ifndef SHOPSCREEN_H
	#define SHOPSCREEN_H

#include "Screen.h"

/* Class ShopScreen: A screen simulating a store, which allows the customer to
 *   buy and sell items using the Money system. Most items are unique, so when
 *   the item is bought or sold, it changes hands between the store and the customer
 */
class ShopScreen : public Screen
{
private:
	SDL_Surface *background;
	SDL_Surface *buttons;
	int selectedStoreItem, selectedClientItem;

	// various graphical messages to inform the user with
	SDL_Surface *msgItem, *msgName, *msgPrice, *msgGold, *msgNoMoney, *msgCurrItemName;
	SDL_Surface *msgCurrItemPrice, *msgQuantity, *msgItemQuantity;

	// tell the user he/she didn't have enough money for the item
	bool showPurchaseError;

	// interactive buttons to invoke the buy or sell process
	Button buyButton;
	Button sellButton;

	// the items currently stocked by the store
	Inventory storeInventory;

	// the coordinates of the various panels within this screen
	static const int ITEMS_PER_ROW = 13;
	static const int SELL_SECTION_X = 2;
	static const int SELL_SECTION_Y = 5;
	static const int BUY_SECTION_X = 2;
	static const int BUY_SECTION_Y = 12;
	static const int INFO_SECTION_X = 18;
	static const int INFO_SECTION_Y = 5;

	bool clickedExit(int,int);
	void paintItem(int, int, Item*);

	// render the informative text for the selected item
	void selectedItem(Item*,bool);

	// frees any memory used for the currently selected item
	void deselectItems();
public:
	ShopScreen(int, int, int, int);
	void paint();
	void mouseLeft(int,int);
	void mouseRight(int,int);

	// signal the parent window that this window wishes to close
	void userExited();
};

#endif