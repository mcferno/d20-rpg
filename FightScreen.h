#ifndef FIGHTSCREEN_H
	#define FIGHTSCREEN_H

#include "Screens.h"

class FightScreen : public Screen
{
private:

	

	SDL_Surface *background;
	SDL_Surface *buttons;

	SDL_Surface *msgInitiativeRoll;
	SDL_Surface *msgAttackRoll;
	SDL_Surface *msgStatic;
	SDL_Surface *msgDynamic;
	
	bool *notifyCompletion;

	Button rollButton;
	Button attackButton;
	Button inventoryButton;
	Button runButton;

	SDL_Color fontColorWhite;
	SDL_Color fontColorRed;
	

	static const int BUTTON_SECTION_X = (12*16);
	static const int BUTTON_SECTION_Y = (24*16);
	static const int INFO_SECTION_X = 10*16;
	static const int INFO_SECTION_Y = 6*16;
	static const int MONSTER_SECTION_X = 21*16;
	static const int MONSTER_SECTION_Y = 6*16;



	bool clickedButton(int,int,Button);
	//bool clickedExit(int,int);
	bool hasRolledInitiative;
	bool initiativeRoll;
	bool attackRoll;
	bool attacked;
	bool hasGeneratedStats;
	bool finished;


	void paintItem(int, int, Item*);

	//some temp holders
	char buffer[4];
	int level, ac, ackbonus, weapondmg;
	int mlevel, mac, mackbonus, mweapondmg;
	char* weaponName; 
	char* mweaponName;


	void paintStaticMessage();
	void paintDynamicMessage();
	void paintStats();

	int playerHp, monsterHp, playerInitiative, monsterInitiative;

	//FOR FIGHT();
	bool damageRoll;
	int playerInitiativeRoll, monsterInitiativeRoll;
	int playerAttackRoll, monsterAttackRoll;
	int playerDamageRoll, monsterDamageRoll;

public:
	FightScreen(int, int, int, int);
	//FightScreen(int, int, int, passCharacter, passMonster)????
	void paint();
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void userExited();

	

	int rollDamageMelee();
	int rollDamageRanged();

	int rollDamageMelee(Monster *thisMonster);

	bool fPlayerAttackRoll(Monster *thisMonster);
	bool fMonsterAttackRoll(Monster *thisMonster);

	void fight(Monster *thisMonster);

};

#endif