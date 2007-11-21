#include "MainGame.h"

MainGame::MainGame(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,672,512);
	numPlayers = -1;
	numEnemies = -1;
	currSpeed = -1;
	currentPlayer = NULL;
	shopScreen = NULL;
	equipScreen = NULL;
	subScreenSignal = false;
	treasure = NULL;
	//weapons = NULL;
	 shopDoorX = shopDoorY = exitDoorX = exitDoorY = -1;
}

// begins the game by loading the level and all of its enemies
void MainGame::init()
{
	loadLevel();
	//weapons = WeaponFactory::getAllWeapons();
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
			std::cout << "Monster #" << i << " rolled a " << rolls[i] << "\n";
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
		currSpeed = 20; // to remove
		paintNow();
	}
	else 
	{
		state = STATE_AI_TURN;

		// kick-start the AI turn timer
		startAITurn = SDL_GetTicks();
		std::cout << "AI's turn ";
	}
}

bool MainGame::isTileOccupied(int xCoord, int yCoord)
{
	if(xCoord == mainCharacter->x && yCoord == mainCharacter->y)
		return true;
	for(int i=0;i<numEnemies;i++)
		if(xCoord == enemies[i].x && yCoord == enemies[i].y)
			return true;
	for(int i=0;i<numTreasure;i++)
		if(xCoord == treasure[i]->x && yCoord == treasure[i]->y)
			return true;
	return false;
}

bool MainGame::isTileWalkable(int xCoord, int yCoord)
{
	if(xCoord >= 0 && xCoord < gameMap.w && yCoord >= 0 && yCoord < gameMap.h)
		return(gameMap.ts[xCoord][yCoord].isWalkable);
	return false;
}

// super simplistic AI, randomly goes up, down, left or right
void MainGame::doAITurn()
{
	std::cout << ".";
	switch(rand() % 4)
	{
		case 0:
			// move up
			if(isTileWalkable(currentPlayer->x,currentPlayer->y-1) && !isTileOccupied(currentPlayer->x,currentPlayer->y-1))
				currentPlayer->y -=1;
			break;
		case 1:
			// move down
			if(isTileWalkable(currentPlayer->x,currentPlayer->y+1) && !isTileOccupied(currentPlayer->x,currentPlayer->y+1))
				currentPlayer->y +=1;
			break;
		case 2:
			// move left
			if(isTileWalkable(currentPlayer->x-1,currentPlayer->y) && !isTileOccupied(currentPlayer->x-1,currentPlayer->y))
				currentPlayer->x -=1;
			break;
		default:
			// move right
			if(isTileWalkable(currentPlayer->x+1,currentPlayer->y) && !isTileOccupied(currentPlayer->x+1,currentPlayer->y))
				currentPlayer->x +=1;
			break;
	}
	// decrement the AI's turn
	currSpeed--;

	// reset the time for the last AI turn played
	if(currSpeed > 0)
		startAITurn = SDL_GetTicks();
}

// initialize the current level, including the map and the enemies
void MainGame::loadLevel()
{
	Graphics *monsterGraphics;
	switch(currentLevel)
	{
		case LEVEL_1:
			std::cout << "\nNow Loading Level 1\n";

			level = new Level();
			level->graphics = ".\\levels\\level01\\graphicTiles.bmp";
			level->index = ".\\levels\\level01\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

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

			numTreasure = 3;
			treasure = new Treasure*[3];
			treasure[0] = new MoneyTreasure(50);
			treasure[0]->x = 16;
			treasure[0]->y = 12;
			treasure[1] = new ItemTreasure((UsableItemFactory::getAllUsableItems())[1]);
			treasure[1]->x = 25;
			treasure[1]->y = 26;
			treasure[2] = new ItemTreasure((UsableItemFactory::getAllUsableItems())[0]);
			treasure[2]->x = 35;
			treasure[2]->y = 25;

			shopDoorX = 4;
			shopDoorY = 6;
			exitDoorX = 38;
			exitDoorY = 1;

			mainCharacter->x = 10;
			mainCharacter->y = 14;

			break;

		case LEVEL_2:
			std::cout << "\nNow Loading Level 2\n";

			level = new Level();
			level->graphics = ".\\levels\\level02\\graphicTiles.png";
			level->index = ".\\levels\\level02\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

			numEnemies = 3;
			enemies = new Monster[numEnemies];

			// worm
			enemies[0].graphics = monsterGraphics;
			enemies[0].clip->x = 8*16;
			enemies[0].clip->y = 1*16;
			enemies[0].clip->w = 16;
			enemies[0].clip->h = 16;
			enemies[0].x = 15;
			enemies[0].y = 20;
			enemies[0].setSpeed(30);

			// evil tree stump
			enemies[1].graphics = monsterGraphics;
			enemies[1].clip->x = 14*16;
			enemies[1].clip->y = 1*16;
			enemies[1].clip->w = 16;
			enemies[1].clip->h = 16;
			enemies[1].x = 25;
			enemies[1].y = 10;
			enemies[1].setSpeed(25);

			// medusa
			enemies[2].graphics = monsterGraphics;
			enemies[2].clip->x = 1*16;
			enemies[2].clip->y = 4*16;
			enemies[2].clip->w = 16;
			enemies[2].clip->h = 16;
			enemies[2].x = 25;
			enemies[2].y = 25;
			enemies[2].setSpeed(20);

			// initialize all treasure and its locations
			numTreasure = 3;
			treasure = new Treasure*[3];
			treasure[0] = new MoneyTreasure(50);
			treasure[0]->x = 16;
			treasure[0]->y = 12;
			treasure[1] = new ItemTreasure((UsableItemFactory::getAllUsableItems())[1]);
			treasure[1]->x = 25;
			treasure[1]->y = 26;
			treasure[2] = new ItemTreasure((UsableItemFactory::getAllUsableItems())[0]);
			treasure[2]->x = 35;
			treasure[2]->y = 25;

			// the tile which allows entry to the shop
			shopDoorX = 4;
			shopDoorY = 6;

			// the tile which leads to the next level
			exitDoorX = 38;
			exitDoorY = 1;

			// main character's starting point
			mainCharacter->x = 10;
			mainCharacter->y = 14;

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

	for(int i=0;i<numEnemies;i++)
		paintObject(&enemies[i]);

	for(int i=0;i<numTreasure;i++)
		paintObject(treasure[i]);

	if(state == STATE_SHOP)
	{
		shopScreen->paint();
	}
	else if(state == STATE_EQUIP)
	{
		equipScreen->paint();
	}
}

void MainGame::paintObject(Object* obj)
{
	if(gameMap.isOnScreen(obj->x,obj->y))
	{
		int x = gameMap.limit.x + (obj->x - gameMap.x)*16;
		int y = gameMap.limit.y + (obj->y - gameMap.y)*16;

		if(currentPlayer == obj)
		{
			applySurface(x, y, highlightTile, screen);
		}

		applySurface(x, y, obj->graphics->image, screen, obj->clip);
	}
}

void MainGame::keyUp()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x,mainCharacter->y-1) && !isTileOccupied(mainCharacter->x,mainCharacter->y-1))
		{
			mainCharacter->y -= 1;
			currSpeed -= 1;

			// temporary
			enterShop();
			exitLevel();
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyDown()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x,mainCharacter->y+1) && !isTileOccupied(mainCharacter->x,mainCharacter->y+1))
		{
			mainCharacter->y += 1;
			currSpeed -= 1;
			
			// temporary
			enterShop();
			exitLevel();
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyLeft()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x-1,mainCharacter->y) && !isTileOccupied(mainCharacter->x-1,mainCharacter->y))
		{
			mainCharacter->x -= 1;
			currSpeed -= 1;
			
			// temporary
			enterShop();
			exitLevel();
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyRight()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x+1,mainCharacter->y) && !isTileOccupied(mainCharacter->x+1,mainCharacter->y))
		{
			mainCharacter->x += 1;
			currSpeed -= 1;
			
			// temporary
			enterShop();
			exitLevel();
		}
		if(currSpeed == 0)
			nextTurn();
	}
}

// handles mouse interactions
void MainGame::mouseLeft(int clickX, int clickY)
{
	if(state == STATE_SHOP)
	{
		shopScreen->mouseLeft(clickX,clickY);

		// if the user closed the shop screen, toggle it to off
		if(subScreenSignal)
		{
			subScreenSignal = false;
			showShop();
		}
	}
	else if(state == STATE_EQUIP)
	{
		equipScreen->mouseLeft(clickX,clickY);

		// if the user closed the shop screen, toggle it to off
		if(subScreenSignal)
		{
			subScreenSignal = false;
			showEquipScreen();
		}
	}
}
void MainGame::mouseRight(int clickX, int clickY)
{
	if(state == STATE_SHOP)
		shopScreen->mouseRight(clickX,clickY);
	else if(state == STATE_EQUIP)
		equipScreen->mouseRight(clickX,clickY);
	else
	{}
}

void MainGame::showShop()
{
	if(state != STATE_SHOP)
	{
		if(shopScreen == NULL)
		{
			shopScreen = new ShopScreen(50,50,448,256);
			shopScreen->setSignal(&subScreenSignal);
		}
		state = STATE_SHOP;
	}
	else
	{
		shopScreen->userExited();

		// you can only enter the shop during the human turn, so restore the human turn
		state = STATE_HUMAN_TURN;
	}
}

void MainGame::showEquipScreen()
{
	if(state != STATE_EQUIP)
	{
		if(equipScreen == NULL)
		{
			equipScreen = new EquipScreen(50,50,352,336);
			equipScreen->setSignal(&subScreenSignal);
		}
		state = STATE_EQUIP;
	}
	else
	{
		equipScreen->userExited();

		// you can only enter the equip screen during the human turn, so restore the human turn
		state = STATE_HUMAN_TURN;
	}
}

void MainGame::openTreasure()
{
	if(numTreasure > 0)
	{
		for(int i=0;i<numTreasure;i++)
		{
			// top, bottom
			if(treasure[i]->x == mainCharacter->x && (treasure[i]->y == mainCharacter->y - 1 || treasure[i]->y == mainCharacter->y + 1))
			{
				treasure[i]->obtainTreasure(mainCharacter);
				break;
			}

			//left right
			if(treasure[i]->y == mainCharacter->y && (treasure[i]->x == mainCharacter->x - 1 || treasure[i]->x == mainCharacter->x + 1))
			{
				treasure[i]->obtainTreasure(mainCharacter);
				break;
			}
		}
	}
}

void MainGame::enterShop()
{
	if(shopDoorX == mainCharacter->x && shopDoorY == mainCharacter->y)
		showShop();
}

// quick post level clean up, and loading of the new level
void MainGame::exitLevel()
{
	if(exitDoorX == mainCharacter->x && exitDoorY == mainCharacter->y && currentLevel == LEVEL_1)
	{
		std::cout << "LEVEL COMPLETED!";
		currentLevel = LEVEL_2;
		
		while(!playOrder.empty())
			playOrder.pop();
		
		currentPlayer = NULL;

		init();
	}
}

void MainGame::tick()
{
	if(state == STATE_AI_TURN)
	{
		if(currSpeed == 0)
		{
			std::cout << " Done!\n";
			nextTurn();
		}
		else
		{
			// only play the AI's turn if enough time has passed
			if((SDL_GetTicks() - startAITurn) > AI_TURN_TIME)
			{
				doAITurn();
				paintNow();
			}
		}
	}
}