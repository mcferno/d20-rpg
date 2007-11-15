#include "Screens.h"
//#include <string>
//using std::string;



// #####################################################################################################

// initialize the static pointers
Character* Screen::mainCharacter = NULL;
SDL_Surface* Screen::screen = NULL;
TTF_Font* Screen::fontCalibri = NULL;
TTF_Font* Screen::fontCalibriBold = NULL;

void Screen::setScreen(SDL_Surface *newScreen)
{
	Screen::screen = newScreen;
}

Screen::Screen(int newX, int newY, int newW, int newH)
{
	x = newX;
	y = newY;
	w = newW;
	h = newH;
}

// #####################################################################################################
// #####################################################################################################

MainGame::MainGame(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,672,512);
	numPlayers = -1;
	numEnemies = -1;
	currSpeed = -1;
	currentPlayer = NULL;
	shopScreen = NULL;
	weapons = NULL;
}

// begins the game by loading the level and all of its enemies
void MainGame::init()
{
	loadLevel();
	weapons = WeaponFactory::getAllWeapons();
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
	std::cout << "\nNow let's do an initiative roll to see who starts\n";
	int *rolls = new int[numPlayers];
	for(int i=0; i<numPlayers; i++)
	{
		rolls[i] = Dice::roll(Dice::D20);
		if(i==0)
			std::cout << "You rolled a " << rolls[i] << "\n";
		else
			std::cout << "Monster #" << i+1 << " rolled a " << rolls[i] << "\n";
	}
	std::cout << "\n";

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

// respect the turn order, handling AI it is an enemy's turn
void MainGame::nextTurn()
{
	if(currentPlayer != NULL)
		playOrder.push(currentPlayer);

	currentPlayer = playOrder.front();
	playOrder.pop();

	currSpeed = currentPlayer->getSpeed();

	if(currentPlayer == mainCharacter)
	{
		state = STATE_HUMAN_TURN;
		std::cout << "Your turn, make your move\n";
	}
	else 
	{
		state = STATE_AI_TURN;
	}

	// play the computer's turn
	if(state == STATE_AI_TURN)
	{
		std::cout << "AI's turn ";
		paintNow();
		SDL_Delay(500);

		for( ;currSpeed>0;currSpeed--)
		{
			std::cout << ".";
			doAITurn();
			paintNow();
			SDL_Delay(250);
		}
		std::cout << " Done!\n";
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

// initialize the current level, including the map and the enemies
void MainGame::loadLevel()
{
	switch(currentLevel)
	{
		case LEVEL_1:
			std::cout << "\nNow Loading Level 1\n";

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
			enemies[0].setSpeed(30);

			// thug
			enemies[1].graphics = monsterGraphics;
			enemies[1].clip->x = 0;
			enemies[1].clip->y = 0;
			enemies[1].clip->w = 16;
			enemies[1].clip->h = 16;
			enemies[1].x = 25;
			enemies[1].y = 10;
			enemies[1].setSpeed(25);

			// porcupine
			enemies[2].graphics = monsterGraphics;
			enemies[2].clip->x = 14*16;
			enemies[2].clip->y = 0;
			enemies[2].clip->w = 16;
			enemies[2].clip->h = 16;
			enemies[2].x = 25;
			enemies[2].y = 25;
			enemies[2].setSpeed(20);

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

	if(state == STATE_SHOP)
	{
		shopScreen->paint();
	}
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

void MainGame::showShop()
{
	if(state != STATE_SHOP)
	{
		std::cout << "SHOW SHOP!\n";
		if(shopScreen == NULL)
			shopScreen = new ShopScreen(50,50,400,200);
		state = STATE_SHOP;
	}
	else
	{
		std::cout << "HIDE SHOP!\n";

		// you can only enter the shop during the human turn, so restore the human turn
		state = STATE_HUMAN_TURN;
	}
}

// #####################################################################################################

ShopScreen::ShopScreen(int newX, int newY, int newW, int newH) : 
	Screen(newX,newY,newW,newH)
{
	bgColor = SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00 );
	weapons = WeaponFactory::getAllWeapons();
	numWeapons = WeaponFactory::getNumWeapons();
	armor = ArmorFactory::getAllArmor();
	numArmor = ArmorFactory::getNumArmor();
}

void ShopScreen::paint()
{
	SDL_Rect tempClip;
	tempClip.x = x;
	tempClip.y = y;
	tempClip.w = w;
	tempClip.h = h;
	SDL_FillRect(screen,&tempClip,bgColor);

	for(int i=0;i<numWeapons;i++)
		applySurface(x+(i*16),y,weapons[i].graphics->image,screen,&weapons[i].graphics->clip[weapons[i].index]);

	for(int i=0;i<numArmor;i++)
		applySurface(x+(i*16),y+16,armor[i].graphics->image,screen,&armor[i].graphics->clip[armor[i].index]);
}


