#include "Screens.h"
//#include <string>
//using std::string;



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

ShopScreen::ShopScreen(int newX, int newY, int newW, int newH) : 
	Screen(newX,newY,newW,newH)
{
	int numWeapons = WeaponFactory::getNumWeapons();
	int numArmor = ArmorFactory::getNumArmor();
	background = loadImage(".\\images\\shopScreen.png");
	buttons = loadImage(".\\images\\buySellButtons.png");
	showPurchaseError = false;
	notifyCompletion = NULL;

	// initialize the buttons used to buy and sell
	buyButton.clip.x = buyButton.clip.y = sellButton.clip.y = 0;
	sellButton.clip.x = 4*16;
	buyButton.clip.w = sellButton.clip.w = 4*16;
	buyButton.clip.h = sellButton.clip.h = 2*16;

	buyButton.x = sellButton.x = x+(INFO_SECTION_X+2)*16;
	buyButton.y = sellButton.y = y+(INFO_SECTION_Y+6)*16;

	fontColorWhite.r = fontColorWhite.g = fontColorWhite.b = 0xFF;

	msgItem = TTF_RenderText_Solid(fontCalibri,"Item:",fontColorWhite);
	msgName = TTF_RenderText_Solid(fontCalibri,"Name:",fontColorWhite);
	msgPrice = TTF_RenderText_Solid(fontCalibri,"Price:",fontColorWhite);
	msgGold = TTF_RenderText_Solid(fontCalibri,"gold",fontColorWhite);
	msgNoMoney = TTF_RenderText_Solid(fontCalibri,"You don't have enough gold to purchase that item!",fontColorWhite);
	msgCurrItemName = NULL;
	msgCurrItemPrice = NULL;

	Weapon** weapons = WeaponFactory::getAllWeapons();
	Armor** armor = ArmorFactory::getAllArmor();

	// make a local copy of the pointers
	for(int i=0;i<numWeapons;i++)
		storeInventory.addItem(weapons[i]);

	for(int i=0;i<numArmor;i++)
		storeInventory.addItem(armor[i]);
	
	selectedStoreItem = selectedClientItem = -1;
}

void ShopScreen::setSignal(bool* signal)
{
	notifyCompletion = signal;
}

void ShopScreen::mouseLeft(int clickX, int clickY)
{
	bool clickResolved = false;

	int col = 0;
	int row = 0;

	int numItems = storeInventory.getSize();
	for(int i=0;i<numItems;i++)
	{
		if(col == ITEMS_PER_ROW)
		{
			row++;
			col = 0;
		}
		if(clickX >= x+(col+SELL_SECTION_X)*16
		 && clickX <= x+(col+SELL_SECTION_X+1)*16
		 && clickY >= y+(row+SELL_SECTION_Y)*16
		 && clickY <= y+(row+SELL_SECTION_Y+1)*16)
		{
			selectedItem(storeInventory.get(i));
			selectedStoreItem = i;
			clickResolved = true;
			break;
		}
		col++;
	}

	if(!clickResolved)
	{
		numItems = mainCharacter->inventory.getSize();
		for(int i=0;i<numItems;i++)
		{
			if(clickX >= x+(i+BUY_SECTION_X)*16
			 && clickX <= x+(i+BUY_SECTION_X+1)*16
			 && clickY >= y+(BUY_SECTION_Y)*16
			 && clickY <= y+(BUY_SECTION_Y+1)*16)
			{
				selectedItem(mainCharacter->inventory.get(i));
				selectedClientItem = i;
				clickResolved = true;
				break;
			}
		}
	}

	if(clickResolved)
		return;

	
	if(selectedStoreItem != -1 && clickedButton(clickX, clickY, buyButton))
	{
		Item *toBuy = storeInventory.get(selectedStoreItem);
		if(mainCharacter->money >= toBuy->cost)
		{
			mainCharacter->money -= toBuy->cost;
			mainCharacter->inventory.addItem(toBuy);
			storeInventory.removeItem(toBuy);
			deselectItems();
		}
		else
		{
			showPurchaseError = true;
		}
		return;
	}
	else if(clickedExit(clickX, clickY))
	{
		*notifyCompletion = true;
	}
	else if(selectedClientItem != -1 && clickedButton(clickX, clickY, sellButton))
	{
		Item *toSell = mainCharacter->inventory.get(selectedClientItem);
		mainCharacter->money += toSell->cost;
		storeInventory.addItem(toSell);
		mainCharacter->inventory.removeItem(toSell);
		deselectItems();
	}
	else { /* no default action */ }
}

bool ShopScreen::clickedButton(int clickX, int clickY, Button toCheck)
{
	// check the bounds of the button
	return (clickX >= toCheck.x && clickX < toCheck.x + toCheck.clip.w && clickY >= toCheck.y && clickY < toCheck.y + toCheck.clip.h);
}

bool ShopScreen::clickedExit(int clickX, int clickY)
{
	return (clickX >= x+w-16 && clickX < x+w && clickY >= y && clickY < y+16);
}

void ShopScreen::selectedItem(Item* item)
{
	if(selectedStoreItem != -1 || selectedClientItem != -1)
		deselectItems();

	msgCurrItemName = TTF_RenderText_Solid(fontCalibriTiny,item->name,fontColorWhite);

	char tempBuffer[5];
	_itoa_s(item->cost.getGold(),tempBuffer,10);
	msgCurrItemPrice = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
}

void ShopScreen::deselectItems()
{
	selectedStoreItem = selectedClientItem = -1;
	if(msgCurrItemName != NULL)
	{
		SDL_FreeSurface(msgCurrItemName);
		msgCurrItemName = NULL;
	}
	if(msgCurrItemPrice != NULL)
	{
		SDL_FreeSurface(msgCurrItemPrice);
		msgCurrItemPrice = NULL;
	}
	showPurchaseError = false;
}

void ShopScreen::mouseRight(int clickX, int clickY)
{
	deselectItems();
}

void ShopScreen::paintItem(int xLoc, int yLoc, Item* item)
{
	applySurface(xLoc,yLoc,item->graphics->image,screen,&item->graphics->clip[item->index]);
}

void ShopScreen::paint()
{
	applySurface(x,y,background,screen);

	int col = 0;
	int row = 0;

	int numItems = storeInventory.getSize();
	for(int i=0;i<numItems;i++)
	{
		if(col == ITEMS_PER_ROW)
		{
			row++;
			col = 0;
		}
		paintItem(x+(col+SELL_SECTION_X)*16,y+(row+SELL_SECTION_Y)*16,storeInventory.get(i));
		
		if(i == selectedStoreItem)
			applySurface(x+(col+SELL_SECTION_X)*16,y+(row+SELL_SECTION_Y)*16,highlightTile,screen);
		
		col++;
	}

	numItems = mainCharacter->inventory.getSize();
	for(int i=0;i<numItems;i++)
	{
		paintItem(x+(i+BUY_SECTION_X)*16,y+BUY_SECTION_Y*16,mainCharacter->inventory.get(i));

		if(i == selectedClientItem)
			applySurface(x+(i+BUY_SECTION_X)*16,y+BUY_SECTION_Y*16,highlightTile,screen);
	}

	if(selectedStoreItem != -1 || selectedClientItem != -1)
	{
		applySurface(x+INFO_SECTION_X*16,y+INFO_SECTION_Y*16,msgItem,screen);
		applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgName,screen);
		applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+3)*16,msgPrice,screen);

		if(msgCurrItemName != NULL)
			applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+2)*16,msgCurrItemName,screen);

		if(msgCurrItemPrice != NULL)
		{
			applySurface(x+(INFO_SECTION_X+3)*16,y+(INFO_SECTION_Y+3)*16,msgCurrItemPrice,screen);
			applySurface(x+(INFO_SECTION_X+5)*16,y+(INFO_SECTION_Y+3)*16,msgGold,screen);
		}

		if(showPurchaseError)
			applySurface(x+(w-msgNoMoney->w)/2,y+h-24,msgNoMoney,screen);

		if(selectedStoreItem != -1)
		{
			paintItem(x+(INFO_SECTION_X+3)*16,y+INFO_SECTION_Y*16,storeInventory.get(selectedStoreItem));
			applySurface(buyButton.x,buyButton.y,buttons,screen,&buyButton.clip);
		}
		else
		{
			paintItem(x+(INFO_SECTION_X+3)*16,y+INFO_SECTION_Y*16,mainCharacter->inventory.get(selectedClientItem));
			applySurface(sellButton.x,sellButton.y,buttons,screen,&sellButton.clip);
		}

	}
}

void ShopScreen::userExited()
{
	deselectItems();
}


