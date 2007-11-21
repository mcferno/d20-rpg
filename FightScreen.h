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

	SDL_Color fontColorWhite;
	SDL_Color fontColorRed;
	

	static const int BUTTON_SECTION_X = 5*16;
	static const int BUTTON_SECTION_Y = 24*16;
	static const int INFO_SECTION_X = 11*16;
	static const int INFO_SECTION_Y = 6*16;
	static const int MONSTER_SECTION_X = 22*16;
	static const int MONSTER_SECTION_Y = 6*16;


	bool clickedButton(int,int,Button);
	//bool clickedExit(int,int);
	bool initiativeRoll;
	bool attackRoll;

	void paintItem(int, int, Item*);

	//some temp holders
	int hp;
	char buffer[4];


	void paintStaticMessage();
	void paintDynamicMessage();
	void paintStats(int, int, bool);

public:
	FightScreen(int, int, int, int);
	//FightScreen(int, int, int, passCharacter, passMonster)????
	void paint();
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void userExited();
	void setSignal(bool*);

	
};

#endif