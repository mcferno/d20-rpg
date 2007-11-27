#ifndef MONEY_H
	#define MONEY_H

/* Class Money: implements the coin system as defined by the d20 system. The
 *   total coin value is stored in the smallest coin value (copper). This is
 *   equivalent to tracking your money in its penny value.
 *
 *   Silver   = 10 Copper Pieces
 *   Gold	  = 100 Copper Pieces
 *   Platinum = 1000 Copper Pieces
 */
class Money
{
private:
	// to convert between the different kinds of coins
	static const int SILVER_TO_COPPER = 10;
	static const int GOLD_TO_COPPER = 100;
	static const int PLATINUM_TO_COPPER = 1000;

	// the only storage unit, all other coin types are calculated on the fly
	int totalCopper;
public:
	// the different types of coins possible
	static const int COPPER = 0;
	static const int SILVER = 1;
	static const int GOLD = 2;
	static const int PLATINUM = 3;

	// create a coin purse of copper, silver, gold, platinum
	Money(int = 0, int = 0, int = 0, int = 0);

	// mutators for each coin type
	void addCopper(int);
	void addSilver(int);
	void addGold(int);
	void addPlatinum(int);

	// adds any kind of coin to the current purse amount
	void addCoin(int,int);

	// mutators to remove each coin type
	void removeCopper(int);
	void removeSilver(int);
	void removeGold(int);
	void removePlatinum(int);

	// accessors for each coin type
	int getCopper();
	int getSilver();
	int getGold();
	int getPlatinum();

	// comparison operators: >, >=, <, <=, ==
	bool Money::operator>(const Money &);
	bool Money::operator>=(const Money &);
	bool Money::operator<(const Money &);
	bool Money::operator<=(const Money &);
	bool Money::operator==(const Money &);

	// += and -= operators
	Money & Money::operator+=(const Money &);
	Money & Money::operator-=(const Money &);
};

#endif