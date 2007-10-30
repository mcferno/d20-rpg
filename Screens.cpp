#include "Screens.h"
#include <iostream>

Graphics *characterSprites = NULL;
Graphics *highlightTile = NULL;
Graphics *selectScreen = NULL;
Character *mainCharacter = NULL;

//int selectedCharacter;
//race selectedRace;
enum playerClass {FIGHTER, MORON};
playerClass myClass;
race myRace;

void SelectionScreen::init()
{
	characterSprites = new Graphics(".\\images\\characters.png");
	selectScreen = new Graphics(".\\images\\selectScreen.png");
	highlightTile = new Graphics(".\\images\\highlight.png");

	//initialize to non-selected
	selectedSprite = -1;
	selectedRace = -1;
	selectedClass = -1;

	//location of available sprites on clip sheet
	availableSprites[0].clip.x = 0;
	availableSprites[0].clip.y = 0; 
	
	availableSprites[1].clip.x = 9*16; 
	availableSprites[1].clip.y = 0; 

	availableSprites[2].clip.x = 0; 
	availableSprites[2].clip.y = 4*16;

	availableSprites[3].clip.x = 5*16; 
	availableSprites[3].clip.y = 4*16;

	availableSprites[4].clip.x = 9*16; 
	availableSprites[4].clip.y = 4*16;

	//location to paint the sprites
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		availableSprites[i].x = 80 + (i*64);
		availableSprites[i].y = 96;
		availableSprites[i].clip.w = availableSprites[i].clip.h = 16; 
	}

	//location to paint the highlight box for seleced races
	availableRaces[0].x = 16;
	availableRaces[0].y = 256;

	availableRaces[1].x = 320;
	availableRaces[1].y = 256;

	availableRaces[2].x = 16;
	availableRaces[2].y = 384;

	availableRaces[3].x = 320;
	availableRaces[3].y = 384;

	//location to paint the highlight box for selected classes
	availableClasses[0].x = 16;
	availableClasses[0].y = 512;
}

SelectionScreen::SelectionScreen()
{
}

SelectionScreen::SelectionScreen(SDL_Surface *newScreen)
{
	screen = newScreen;
	init();
}

void SelectionScreen::setSignal(bool* signal)
{
	signalCompletion = signal;
}

void SelectionScreen::paint()
{
	// paint background
	// if player graphic selected? paint in selected character area
	// if race selected? 

	// apply background image
	applySurface(0,0,selectScreen->image,screen);

	// paint each sprite
	for(int i=0;i<NUM_CHARACTERS;i++)
		paintGraphicsSelection(availableSprites[i]);

	// highlight a selected sprite
	if(selectedSprite != -1) 
		applySurface(availableSprites[selectedSprite].x,availableSprites[selectedSprite].y,highlightTile->image,screen);
	if(selectedRace != -1)
		applySurface(availableRaces[selectedRace].x,availableRaces[selectedRace].y,highlightTile->image,screen);
	if(selectedClass != -1)
		applySurface(availableClasses[selectedClass].x,availableClasses[selectedClass].y,highlightTile->image,screen);
}

void SelectionScreen::paintGraphicsSelection(GraphicsSelection &ss)
{
	applySurface(ss.x,ss.y,characterSprites->image,screen,&ss.clip);
}

void SelectionScreen::mouseLeft(int x, int y)
{
	//what happens when you select START
	if (x >= 672 && x <= 784 && y >= 544 && y <= 591)
	{
		switch (selectedRace) {
			case 0:
				myRace = HUMAN;
				std::cout << "\nhuman";
				break;
			case 1:
				myRace = DWARF;
				std::cout << "\ndwarf";
				break;
			case 2:
				myRace = ELF;
				std::cout << "\nelf";
				break;
			case 3:
				myRace = GNOME;
				std::cout << "\ngnome";
		}
		switch (selectedClass) {
			case 0:
				myClass = FIGHTER;
				std::cout << "\nfighter";
				break;
			case -1:
				break;
		}
		SDL_Rect *characterRect = new SDL_Rect();

		switch (myClass) {
			case FIGHTER:
				mainCharacter = new Fighter(myRace);
				mainCharacter->graphics = characterSprites;

				characterRect->x = availableSprites[selectedSprite].clip.x;
				characterRect->y = availableSprites[selectedSprite].clip.y;
				characterRect->w = availableSprites[selectedSprite].clip.w;
				characterRect->h = availableSprites[selectedSprite].clip.h;
					
				mainCharacter->x = 10;
				mainCharacter->y = 14;

				mainCharacter->clip = characterRect;

				std::cout << "\nYOU ARE A " << myRace << " " << myClass <<"\n and you selected character #" << selectedSprite;
				break;
			case MORON:
				std::cout << "WHY?!?!";

		}

		*signalCompletion = true;
	}

	if (x >= 672 && x <= 784 && y >= 480 && y <= 527)
	{
		selectedSprite = -1;
		selectedRace = -1;
		selectedClass = -1;
	}




	// check if any of the sprites were selected
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		if (x>=availableSprites[i].x && x<=(availableSprites[i].x+16) && y>=availableSprites[i].y && y<=(availableSprites[i].y+16))
			selectedSprite = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_RACES;i++)
	{
		if (x>=availableRaces[i].x && x<=(availableRaces[i].x+16) && y>=availableRaces[i].y && y<=(availableRaces[i].y+16))
			selectedRace = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_CLASSES;i++)
	{
		if (x>=availableClasses[i].x && x<=(availableClasses[i].x+16) && y>=availableClasses[i].y && y<=(availableClasses[i].y+16))
			selectedClass = i;
	}
}

void SelectionScreen::mouseRight(int x, int y)
{
	selectedSprite = -1;
	selectedRace = -1;
	selectedClass = -1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// temporary until the characters are properly created
int currSpeed = -1;

MainGame::MainGame()
{
}

MainGame::MainGame(SDL_Surface *newScreen)
{
	screen = newScreen;
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,672,512);
	numPlayers = -1;
	numEnemies = -1;
	currentPlayer = NULL;
}

void MainGame::init()
{
	loadLevel();
	numPlayers = numEnemies+1;

	gameMap.loadGraphics(level->graphics, level->alphaR, level->alphaG, level->alphaB);
	gameMap.parseIndex(level->index);
	state = STATE_LEVEL_START;

	std::cout << "There are " << numEnemies << " monsters on this level\n";

	if(state == STATE_LEVEL_START)
		doInitiativeRoll();
}

// initiative roll to decide the order the turns will be taken in
// the first roll is arbitarily the human's roll
void MainGame::doInitiativeRoll()
{
	std::cout << "Now let's do an initiative roll to see who starts\n";
	int *rolls = new int[numPlayers];
	for(int i=0; i<numPlayers; i++)
	{
		rolls[i] = Dice::roll(Dice::D20);
		if(i==0)
			std::cout << "You rolled a " << rolls[i] << "\n";
		else
			std::cout << "Monster #" << i+1 << " rolled a " << rolls[i] << "\n";
	}

	int i, j, highestRoll, highRollerIdx;

	// put the characters in the queue in the order of their initiative roll
	for(i=0;i<numPlayers;i++)
	{
		// arbitrarily choose the first roll as the highest
		highestRoll = rolls[0];
		highRollerIdx = 0;

		// see if any of the other rolls were higher
		for(j=1;j<numPlayers;j++)
		{
			if(rolls[j] > highestRoll)
			{
				highestRoll = rolls[j];
				highRollerIdx = j;
			}
		}
		// set the highest to -1, so it doesn't appear again
		rolls[highRollerIdx] = -1;

		// add the highest roller to the queue
		if(highRollerIdx == 0)
			playOrder.push(mainCharacter);
		else
			playOrder.push(&enemies[highRollerIdx-1]);
	}

	nextTurn();
}

void MainGame::nextTurn()
{
	currSpeed = 5;

	if(currentPlayer != NULL)
		playOrder.push(currentPlayer);

	currentPlayer = playOrder.front();
	playOrder.pop();

	if(currentPlayer == mainCharacter)
	{
		state = STATE_HUMAN_TURN;
		std::cout << "Your turn, make your move\n";
	}
	else 
	{
		state = STATE_AI_TURN;
		std::cout << "AI's turn\n";
	}

	if(state == STATE_AI_TURN)
	{
		for(;currSpeed>=0;currSpeed--)
		{
			doAITurn();
			paintNow();
			SDL_Delay(250);
		}
		nextTurn();
	}
}

// super simplistic AI, randomly goes up, down, left or right
void MainGame::doAITurn()
{
	switch(rand() % 4)
	{
		case 0:
			// move up
			if(currentPlayer->y > 0)
				if(gameMap.ts[currentPlayer->x][currentPlayer->y-1].isWalkable)
					currentPlayer->y -=1;
			break;
		case 1:
			// move down
			if(currentPlayer->y < gameMap.h-1)
				if(gameMap.ts[currentPlayer->x][currentPlayer->y+1].isWalkable)
					currentPlayer->y +=1;
			break;
		case 2:
			// move left
			if(currentPlayer->x > 0)
				if(gameMap.ts[currentPlayer->x-1][currentPlayer->y].isWalkable)
					currentPlayer->x -=1;
			break;
		default:
			// move right
			if(currentPlayer->x < gameMap.w-1)
				if(gameMap.ts[currentPlayer->x+1][currentPlayer->y].isWalkable)
					currentPlayer->x +=1;
			break;
	}
}

void MainGame::loadLevel()
{
	switch(currentLevel)
	{
		case LEVEL_1:
			std::cout << "Now Loading Level 1\n";

			level = new Level();
			level->graphics = ".\\levels\\level00\\graphicTiles.bmp";
			level->index = ".\\levels\\level00\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			Graphics *monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

			numEnemies = 3;

			enemies = new Monster[numEnemies];

			// skeleton
			enemies[0].graphics = monsterGraphics;
			enemies[0].clip->x = 6*16;
			enemies[0].clip->y = 2*16;
			enemies[0].clip->w = 16;
			enemies[0].clip->h = 16;
			enemies[0].x = 15;
			enemies[0].y = 20;

			// thug
			enemies[1].graphics = monsterGraphics;
			enemies[1].clip->x = 0;
			enemies[1].clip->y = 0;
			enemies[1].clip->w = 16;
			enemies[1].clip->h = 16;
			enemies[1].x = 25;
			enemies[1].y = 10;

			// porcupine
			enemies[2].graphics = monsterGraphics;
			enemies[2].clip->x = 14*16;
			enemies[2].clip->y = 0;
			enemies[2].clip->w = 16;
			enemies[2].clip->h = 16;
			enemies[2].x = 25;
			enemies[2].y = 25;

			break;
	}
}

void MainGame::paintNow()
{
	paint();
	SDL_Flip(screen);
}

void MainGame::paint()
{
	gameMap.paint();

	paintObject(mainCharacter);

	paintObject(&enemies[0]);
	paintObject(&enemies[1]);
	paintObject(&enemies[2]);
}

void MainGame::paintObject(Object* obj)
{
	if(gameMap.isOnScreen(obj->x,obj->y))
	{
		int x = gameMap.limit.x + (obj->x - gameMap.x)*16;
		int y = gameMap.limit.y + (obj->y - gameMap.y)*16;
		applySurface(x, y, obj->graphics->image, screen, obj->clip);
	}
}

void MainGame::keyUp()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->y > 0)
		{
			if(gameMap.ts[mainCharacter->x][mainCharacter->y-1].isWalkable)
			{
				mainCharacter->y -= 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyDown()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->y < gameMap.h-1)
		{
			if(gameMap.ts[mainCharacter->x][mainCharacter->y+1].isWalkable)
			{
				mainCharacter->y += 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyLeft()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->x > 0)
		{
			if(gameMap.ts[mainCharacter->x-1][mainCharacter->y].isWalkable)
			{
				mainCharacter->x -= 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyRight()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->x < gameMap.w-1)
		{
			if(gameMap.ts[mainCharacter->x+1][mainCharacter->y].isWalkable)
			{
				mainCharacter->x += 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
