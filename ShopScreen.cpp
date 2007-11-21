#include "ShopScreen.h"

ShopScreen::ShopScreen(int newX, int newY, int newW, int newH) : 
	Screen(newX,newY,newW,newH)
{
	int numWeapons = WeaponFactory::getNumWeapons();
	int numArmor = ArmorFactory::getNumArmor();
	int numItems = UsableItemFactory::getNumUsableItems();

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
	msgPrice = TTF_RenderText_Solid(fontCalibri,"Price: ",fontColorWhite);
	msgGold = TTF_RenderText_Solid(fontCalibri," gold",fontColorWhite);
	msgNoMoney = TTF_RenderText_Solid(fontCalibri,"You don't have enough gold to purchase that item!",fontColorWhite);
	msgQuantity = TTF_RenderText_Solid(fontCalibri,"Quantity: ",fontColorWhite);
	msgCurrItemName = NULL;
	msgCurrItemPrice = NULL;
	msgItemQuantity = NULL;

	Weapon** weapons = WeaponFactory::getAllWeapons();
	Armor** armor = ArmorFactory::getAllArmor();
	UsableItem** items = UsableItemFactory::getAllUsableItems();

	// make a local copy of the pointers
	for(int i=0;i<numWeapons;i++)
		storeInventory.addItem(weapons[i]);

	for(int i=0;i<numArmor;i++)
		storeInventory.addItem(armor[i]);

	for(int i=0;i<numItems;i++)
		storeInventory.addItem(items[i]);
	
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
			selectedItem(storeInventory.get(i),true);
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
				selectedItem(mainCharacter->inventory.get(i),false);
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
			mainCharacter->addItem(toBuy);

			if(!toBuy->isItemOfQuantity())
			{
				storeInventory.removeItem(toBuy);
			}
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
		if(!toSell->isItemOfQuantity())
		{
			mainCharacter->money += toSell->cost;
			storeInventory.addItem(toSell);
			mainCharacter->removeItem(toSell);
			deselectItems();
		}
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

void ShopScreen::selectedItem(Item* item, bool belongsToShop)
{
	if(selectedStoreItem != -1 || selectedClientItem != -1)
		deselectItems();

	msgCurrItemName = TTF_RenderText_Solid(fontCalibriTiny,item->name,fontColorWhite);

	char tempBuffer[5];
	_itoa_s(item->cost.getGold(),tempBuffer,10);
	msgCurrItemPrice = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);

	if(item->isItemOfQuantity())
	{
		UsableItem* usableItem = static_cast<UsableItem*>(item);
		if(usableItem)
		{
			if(belongsToShop)
				_itoa_s(usableItem->numLeft(),tempBuffer,10);
			else
			{
				switch(usableItem->usableType)
				{
					case UsableItem::POTION:
						_itoa_s(mainCharacter->getNumPotions(),tempBuffer,10);
						break;
					case UsableItem::ARROW:
						_itoa_s(mainCharacter->getNumArrows(),tempBuffer,10);
						break;
					case UsableItem::BOLT:
						_itoa_s(mainCharacter->getNumArrows(),tempBuffer,10);
						break;
					default:
						_itoa_s(0,tempBuffer,10);
						break;
				}
			}
			msgItemQuantity = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
		}
	}
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
	if(msgItemQuantity != NULL)
	{
		SDL_FreeSurface(msgItemQuantity);
		msgItemQuantity = NULL;
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
			applySurface(x+msgPrice->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+3)*16,msgCurrItemPrice,screen);
			applySurface(x+msgPrice->w+msgCurrItemPrice->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+3)*16,msgGold,screen);
		}

		if(msgItemQuantity != NULL)
		{
			applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+4)*16,msgQuantity,screen);
			applySurface(x+msgQuantity->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+4)*16,msgItemQuantity,screen);
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
			Item* temp = mainCharacter->inventory.get(selectedClientItem);
			paintItem(x+(INFO_SECTION_X+3)*16,y+INFO_SECTION_Y*16,temp);
			if(!temp->isItemOfQuantity())
				applySurface(sellButton.x,sellButton.y,buttons,screen,&sellButton.clip);
		}

	}
}

void ShopScreen::userExited()
{
	deselectItems();
}