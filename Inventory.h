#ifndef INVENTORY_H
	#define INVENTORY_H

#include <vector>
#include "Items.h"
using std::vector;

/* Class Inventory: a collection of items. The inventory can be modified by
 *   adding or removing items.
 */
class Inventory
{
private:
	// the set of items being stored
	vector<Item *> itemStorage;
public:	
	Inventory();

	// adds a new item to the inventory
	void addItem(Item*);

	// removes an item from the inventory
	void removeItem(Item*);

	// how many items are in the inventory
	int getSize();

	// same as getSize but returns an unsigned int
	unsigned int getSizeU();

	// returns a pointer to an item present in the inventory
	Item* get(unsigned int);
};

#endif