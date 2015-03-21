#include "Inventory.h"

Inventory::Inventory() {}

void Inventory::addItem(Item* newItem)
{
	// add the new item at the end of the vector
	itemStorage.push_back(newItem);
}

void Inventory::removeItem(Item *toRemove)
{
	// check to see if the inventory is empty
	if(!itemStorage.empty())
	{
		// find the item to remove, if present
		for(unsigned int i=0; i<itemStorage.size();i++)
		{
			// check if you've found the item you were looking for
			if(toRemove == itemStorage[i])
			{
				itemStorage.erase(itemStorage.begin()+i);
				break;
			}
		}
	}
}

int Inventory::getSize()
{
	return static_cast<int>(itemStorage.size());
}

unsigned int Inventory::getSizeU()
{
	return itemStorage.size();
}

Item* Inventory::get(unsigned int index)
{
	// ensure the index is in a valid range
	if(index >= 0 && index < itemStorage.size())
		return itemStorage.at(index);
	return NULL;
}