#include "ShopScreen.h"

ShopScreen::ShopScreen(int newX, int newY, int newW, int newH) :
	Screen(newX,newY,newW,newH)
{
	background = loadImage(SHOP_SCREEN_BACKDROP);
	buttons = loadImage(UI_SHOP_BUTTONS);
	showPurchaseError = false;

	// initialize the buttons used to buy and sell
	buyButton = Button(x+(INFO_SECTION_X+2)*TILE_SIZE, y+(INFO_SECTION_Y+6)*TILE_SIZE, 4*TILE_SIZE, 2*TILE_SIZE, 0, 0, buttons);
	sellButton = Button(x+(INFO_SECTION_X+2)*TILE_SIZE, y+(INFO_SECTION_Y+6)*TILE_SIZE, 4*TILE_SIZE, 2*TILE_SIZE, 4*TILE_SIZE, 0, buttons);

	// render the fixed informative text
	msgItem = TTF_RenderText_Solid(fontCalibri,"Item:",colorWhite);
	msgName = TTF_RenderText_Solid(fontCalibri,"Name:",colorWhite);
	msgPrice = TTF_RenderText_Solid(fontCalibri,"Price: ",colorWhite);
	msgGold = TTF_RenderText_Solid(fontCalibri," gold",colorWhite);
	msgNoMoney = TTF_RenderText_Solid(fontCalibri,"You don't have enough gold to purchase that item!",colorWhite);
	msgQuantity = TTF_RenderText_Solid(fontCalibri,"Quantity: ",colorWhite);

	// variable informative text will be initialized when an item is selected
	msgCurrItemName = NULL;
	msgCurrItemPrice = NULL;
	msgItemQuantity = NULL;

	// get a array of pointers for all available weapons, armor and items in the game
	Weapon** weapons = WeaponFactory::getAllWeapons();
	Armor** armor = ArmorFactory::getAllArmor();
	UsableItem** items = UsableItemFactory::getAllUsableItems();

	// how many of each kind of item is there?
	int numWeapons = WeaponFactory::getNumWeapons();
	int numArmor = ArmorFactory::getNumArmor();
	int numItems = UsableItemFactory::getNumUsableItems();

	// make a local copy of the pointers for all items to be available in the store
	for(int i=0;i<numWeapons;i++)
		storeInventory.addItem(weapons[i]);
	for(int i=0;i<numArmor;i++)
		storeInventory.addItem(armor[i]);
	for(int i=0;i<numItems;i++)
		storeInventory.addItem(items[i]);

	// no items are selected yet
	selectedStoreItem = selectedClientItem = -1;
}

void ShopScreen::mouseLeft(int clickX, int clickY)
{
	int col = 0;
	int row = 0;
	int numItems = storeInventory.getSize();

	// check to see if one of the store's items were clicked
	for(int i=0;i<numItems;i++)
	{
		if(col == ITEMS_PER_ROW)
		{
			row++;
			col = 0;
		}

		// item was clicked, set it as the selected store item
		if(clickX >= x+(col+SELL_SECTION_X)*16
		 && clickX <= x+(col+SELL_SECTION_X+1)*16
		 && clickY >= y+(row+SELL_SECTION_Y)*16
		 && clickY <= y+(row+SELL_SECTION_Y+1)*16)
		{
			selectedItem(storeInventory.get(i),true);
			selectedStoreItem = i;
			return;
		}
		col++;
	}

	// check if an item in user's inventory was clicked
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
			return;
		}
	}


	// check if the user wishes to purchase the selected store item
	if(selectedStoreItem != -1 && buyButton.inBounds(clickX, clickY))
	{
		Item *toBuy = storeInventory.get(selectedStoreItem);

		// process the transaction if the user has enough money
		if(mainCharacter->money >= toBuy->cost)
		{
			mainCharacter->money -= toBuy->cost;
			mainCharacter->addItem(toBuy);

			// don't remove item which have quantities (like arrows)
			if(!toBuy->isItemOfQuantity())
			{
				storeInventory.removeItem(toBuy);
			}
			deselectItems();
		}
		// inform the user he/she doesn't have enough money to buy that item
		else
		{
			showPurchaseError = true;
		}
		return;
	}
	// the user wishes to exit, signal the parent window
	else if(clickedExit(clickX, clickY))
	{
		signalCompletion();
	}
	// the user wishes to sell one of his/her items, process the transaction
	else if(selectedClientItem != -1 && sellButton.inBounds(clickX, clickY))
	{
		Item *toSell = mainCharacter->inventory.get(selectedClientItem);

		// you can't sell an item of quantity
		if(!toSell->isItemOfQuantity())
		{
			mainCharacter->money += toSell->cost;
			storeInventory.addItem(toSell);
			mainCharacter->removeItem(toSell);
			deselectItems();
		}
	}
}

bool ShopScreen::clickedExit(int clickX, int clickY)
{
	// was the click in the top right corner (where the exit button is)
	return (clickX >= x+w-16 && clickX < x+w && clickY >= y && clickY < y+16);
}

void ShopScreen::selectedItem(Item* item, bool belongsToShop)
{
	// clear any old text dependent data
	if(selectedStoreItem != -1 || selectedClientItem != -1)
		deselectItems();

	msgCurrItemName = TTF_RenderText_Solid(fontCalibriTiny,item->name,colorWhite);

	std::string tempBuffer;
	tempBuffer = to_string(item->cost.getGold());
	msgCurrItemPrice = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), colorWhite);

	// get special information about the item if it has a quantity
	if(item->isItemOfQuantity())
	{
		UsableItem* usableItem = static_cast<UsableItem*>(item);
		if(usableItem)
		{
			if(belongsToShop)
				tempBuffer = to_string(usableItem->numLeft());
			else
			{
				// check how many of this item the character is carrying on him
				switch(usableItem->usableType)
				{
					case UsableItem::POTION:
						tempBuffer = to_string(mainCharacter->getNumPotions());
						break;
					case UsableItem::ARROW:
						tempBuffer = to_string(mainCharacter->getNumArrows());
						break;
					case UsableItem::BOLT:
						tempBuffer = to_string(mainCharacter->getNumBolts());
						break;
					default:
						tempBuffer = to_string(0);
						break;
				}
			}
			msgItemQuantity = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), colorWhite);
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

	// paint all of the store's items
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
	// paint all of the customer's items
	for(int i=0;i<numItems;i++)
	{
		paintItem(x+(i+BUY_SECTION_X)*16,y+BUY_SECTION_Y*16,mainCharacter->inventory.get(i));

		if(i == selectedClientItem)
			applySurface(x+(i+BUY_SECTION_X)*16,y+BUY_SECTION_Y*16,highlightTile,screen);
	}

	// paint more information if there is an item currently selected
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
