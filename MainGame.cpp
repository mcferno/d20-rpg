#include "MainGame.h"

MainGame::MainGame(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	currentLevel = LEVEL_1;
	gameMap = Map(TILE_SIZE,TILE_SIZE,37*TILE_SIZE,28*TILE_SIZE);
	numPlayers = -1;
	numEnemies = -1;
	currSpeed = -1;
	currentPlayer = NULL;
	shopScreen = NULL;
	equipScreen = NULL;
	subScreenSignal = false;
	treasure = NULL;
	selectedEnemy = NULL;
	shopDoorX = shopDoorY = exitDoorX = exitDoorY = -1;

	background = loadImage(".\\images\\mainScreen.png");

	SDL_Surface *arrows = loadImage(".\\images\\arrows.png",0xFF,0x0,0xFF);

	// initialize the 4 directional scrolling arrows for the custom map
	//left
	mapBtns[0] = Button(TILE_SIZE,29*TILE_SIZE,TILE_SIZE,TILE_SIZE,0,0,arrows);
	//right
	mapBtns[1] = Button(37*TILE_SIZE,29*TILE_SIZE,TILE_SIZE,TILE_SIZE,TILE_SIZE,0,arrows);
	//up
	mapBtns[2] = Button(38*TILE_SIZE,TILE_SIZE,TILE_SIZE,TILE_SIZE,2*TILE_SIZE,0,arrows);
	//down
	mapBtns[3] = Button(38*TILE_SIZE,28*TILE_SIZE,TILE_SIZE,TILE_SIZE,3*TILE_SIZE,0,arrows);

	SDL_Surface *buttons = loadImage(".\\images\\mainGameButtons.png");
	// attack button
	controlBtns[0] = Button(INFO_PANEL_X+TILE_SIZE,INFO_PANEL_Y+TILE_SIZE*4,5*TILE_SIZE,2*TILE_SIZE,0,0,buttons);
	// equip screen button
	controlBtns[1] = Button(INFO_PANEL_X,INFO_PANEL_Y+TILE_SIZE*18,7*TILE_SIZE,2*TILE_SIZE,controlBtns[0].w,0,buttons);
	// open chest button
	controlBtns[2] = Button(INFO_PANEL_X,INFO_PANEL_Y+TILE_SIZE*15,7*TILE_SIZE,2*TILE_SIZE,12*TILE_SIZE,0,buttons);
	// end turn button
	controlBtns[3] = Button(INFO_PANEL_X,INFO_PANEL_Y+TILE_SIZE*21,7*TILE_SIZE,2*TILE_SIZE,19*TILE_SIZE,0,buttons);

	exitBtn.x = INFO_PANEL_X+TILE_SIZE;
	exitBtn.y = INFO_PANEL_Y+TILE_SIZE*24;
	exitBtn.w = TILE_SIZE*5;
	exitBtn.h = TILE_SIZE*2;

	msgInfo[0] = TTF_RenderText_Solid(fontCalibri, "Monster ", colorWhite );
	msgInfo[1] = TTF_RenderText_Solid(fontCalibri, "HP: ", colorWhite );

	walkableHighlight = loadImage(".\\images\\fullHighlight.png");
	targetableHighlight = loadImage(".\\images\\fullHighlightWeapon.png");
	SDL_SetAlpha( walkableHighlight, SDL_SRCALPHA | SDL_RLEACCEL, 75 );
	SDL_SetAlpha( targetableHighlight, SDL_SRCALPHA | SDL_RLEACCEL, 75 );
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
		attackedThisRound = false;
		std::cout << "Your turn, make your move\n";
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
			level->graphics = LEVEL_1_GRAPHICS;
			level->index = LEVEL_1_MAP;
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

			numEnemies = 3;
			enemies = new Monster[numEnemies];
			enemies[0] = Monster(15,20,6,2,monsterGraphics,Monster::SKELETON,1);
			enemies[1] = Monster(25,10,0,0,monsterGraphics,Monster::ELF,1);
			enemies[2] = Monster(25,15,14,0,monsterGraphics,Monster::GOBLIN,1);

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

			numEnemies = 6;
			enemies = new Monster[numEnemies];
			enemies[0] = Monster(15,20,6,2,monsterGraphics,Monster::SKELETON,2);
			enemies[1] = Monster(25,10,0,0,monsterGraphics,Monster::ELF,1);
			enemies[2] = Monster(25,25,14,0,monsterGraphics,Monster::GOBLIN,1);
			enemies[3] = Monster(20,15,8,1,monsterGraphics,Monster::LIZARD,2);
			enemies[4] = Monster(25,15,14,1,monsterGraphics,Monster::VINE,1);
			enemies[5] = Monster(15,15,1,4,monsterGraphics,Monster::MEDUSA,5);

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
	applySurface(x,y,background,screen);

	// move the window if the current player isn't on screen
	if(currentPlayer != NULL)
	{
		if(!gameMap.isOnScreen(currentPlayer->x,currentPlayer->y))
		{
			while(currentPlayer->x < gameMap.x)
				gameMap.scrollLeft();
			while(currentPlayer->x > gameMap.x+gameMap.limitWTiles-1)
				gameMap.scrollRight();
			while(currentPlayer->y < gameMap.y)
				gameMap.scrollUp();
			while(currentPlayer->y > gameMap.y+gameMap.limitHTiles-1)
				gameMap.scrollDown();
		}
	}

	gameMap.paint();

	if(gameMap.limit.w < gameMap.w*TILE_SIZE)
	{
		mapBtns[0].paint();
		mapBtns[1].paint();
	}

	if(gameMap.limit.h < gameMap.h*TILE_SIZE)
	{
		mapBtns[2].paint();
		mapBtns[3].paint();
	}

	if(state == STATE_HUMAN_TURN)
	{
		paintRange(currSpeed,walkableHighlight);
		paintRange(mainCharacter->getWeaponRange(),targetableHighlight,true);
		
		paintInfoPanel();
	}

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

void MainGame::paintRange(int dist, SDL_Surface *highlight, bool ignoreUnWalkable)
{
	int center = dist;

	if(center < 1)
		return;

	int centerX = mainCharacter->x;
	int centerY = mainCharacter->y;

	unsigned short unwalkable = 65535;
	unsigned short uninit = 9999;

	int i,j;

	int width = center*2+1;
	unsigned short **b;
	b = new unsigned short*[width];

	// first pass, initialization;
	// goes row by row
	for(i=0;i<width;i++)
	{
		b[i] = new unsigned short[width];
		for(j=0;j<width;j++)
		{
			if(gameMap.isOnScreen(centerX+(center-i),centerY+(center-j)) 
				&& (!isTileOccupied(centerX+(center-i),centerY+(center-j)) || ignoreUnWalkable)
				&& (isTileWalkable(centerX+(center-i),centerY+(center-j)) || ignoreUnWalkable))
				b[i][j] = uninit;
			else
				b[i][j] = unwalkable;
		}
	}

	// set the center to 0
	b[center][center] = 0;

	unsigned short min;

	// first discovery path
	for(int k=0;k<center;k++)
	{
		for(i=0;i<width;i++)
		{
			for(j=0;j<width;j++)
			{
				min = unwalkable;
				
				if(i>0)
					min = (b[i-1][j] < min)? b[i-1][j] : min;
				if(i<width-1)
					min = (b[i+1][j] < min)? b[i+1][j] : min;
				if(j>0)
					min = (b[i][j-1] < min)? b[i][j-1] : min;
				if(j<width-1)
					min = (b[i][j+1] < min)? b[i][j+1] : min;

				if(min < uninit && b[i][j] != unwalkable && b[i][j] > min)
					b[i][j] = min+1;
			}
		}
	}

	// last step, highlight walkable tiles
	for(i=0;i<width;i++)
	{
		for(j=0;j<width;j++)
		{
			// valid tile
			if(b[i][j] <= center)
			{
				applySurface(tileToPixelsX(centerX+(center-i)),tileToPixelsY(centerY+(center-j)),highlight,screen);
			}
		}
	}

}

void MainGame::paintObject(Object* obj)
{
	if(gameMap.isOnScreen(obj->x,obj->y))
	{
		int x = tileToPixelsX(obj->x);
		int y = tileToPixelsY(obj->y);

		if(currentPlayer == obj)
		{
			applySurface(x, y, highlightTile, screen);
		}

		applySurface(x, y, obj->graphics->image, screen, obj->clip);
	}
}

void MainGame::paintInfoPanel()
{
	if(selectedEnemy != NULL)
	{
		applySurface(x+INFO_PANEL_X,y+INFO_PANEL_Y,msgInfo[0],screen);
		applySurface(x+INFO_PANEL_X+msgInfo[0]->w,y+INFO_PANEL_Y,selectedEnemy->graphics->image,screen,selectedEnemy->clip);
		applySurface(x+INFO_PANEL_X,y+INFO_PANEL_Y+TILE_SIZE,msgCustomInfo[0],screen);
		applySurface(x+INFO_PANEL_X,y+INFO_PANEL_Y+TILE_SIZE*2,msgInfo[1],screen);
		applySurface(x+INFO_PANEL_X+msgInfo[1]->w,y+INFO_PANEL_Y+TILE_SIZE*2,msgCustomInfo[1],screen);

		if(inRange(selectedEnemy,mainCharacter->getWeaponRange()) && !attackedThisRound)
			controlBtns[0].paint();
	}
	controlBtns[1].paint();
	controlBtns[3].paint();

	for(int i=0;i<numTreasure;i++)
	{
		if(inRange(treasure[i],1) && !treasure[i]->isOpen())
			controlBtns[2].paint();
	}
}

bool MainGame::inRange(Object* target, int range)
{
	return (range >= abs(mainCharacter->x-target->x) + abs(mainCharacter->y-target->y));
}

bool MainGame::clickedEnemy(int clickX, int clickY)
{
	int xCoord, yCoord;
	for(int i=0;i<numEnemies;i++)
	{
		xCoord = tileToPixelsX(enemies[i].x);
		yCoord = tileToPixelsY(enemies[i].y);

		if(clickX >= xCoord && clickX <= xCoord + TILE_SIZE && clickY >= yCoord && clickY <= yCoord + TILE_SIZE)
		{
			selectEnemy(&enemies[i]);
			return true;
		}

	}
	return false;
}

void MainGame::selectEnemy(Monster* toSelect)
{
	if(selectedEnemy != toSelect)
	{
		if(selectedEnemy != NULL)
			unselectEnemy();

		selectedEnemy = toSelect;

		msgCustomInfo[0] = TTF_RenderText_Solid(fontCalibri, selectedEnemy->getName(), colorWhite );

		char tempBuffer[5];
		_itoa_s(selectedEnemy->getHp(),tempBuffer,10);
		msgCustomInfo[1] = TTF_RenderText_Solid(fontCalibri, tempBuffer, colorWhite );
	}
}

void MainGame::unselectEnemy()
{
	selectedEnemy = NULL;

	if(msgCustomInfo[0] != NULL)
	{
		SDL_FreeSurface(msgCustomInfo[0]);
		msgCustomInfo[0] = NULL;
	}
	if(msgCustomInfo[1] != NULL)
	{
		SDL_FreeSurface(msgCustomInfo[1]);
		msgCustomInfo[1] = NULL;
	}
}

int MainGame::tileToPixelsX(int tx)
{
	return gameMap.limit.x + (tx - gameMap.x)*16;
}

int MainGame::tileToPixelsY(int ty)
{
	return gameMap.limit.y + (ty - gameMap.y)*16;
}

void MainGame::characterMoved()
{
	if(state == STATE_HUMAN_TURN)
	{
		currSpeed -= 1;
		enterShop();
		exitLevel();
	}
}

void MainGame::keyUp()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x,mainCharacter->y-1) && !isTileOccupied(mainCharacter->x,mainCharacter->y-1) && currSpeed > 0)
		{
			mainCharacter->y -= 1;
			characterMoved();
		}
	}
}
void MainGame::keyDown()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x,mainCharacter->y+1) && !isTileOccupied(mainCharacter->x,mainCharacter->y+1) && currSpeed > 0)
		{
			mainCharacter->y += 1;
			characterMoved();
		}
	}
}
void MainGame::keyLeft()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x-1,mainCharacter->y) && !isTileOccupied(mainCharacter->x-1,mainCharacter->y) && currSpeed > 0)
		{
			mainCharacter->x -= 1;
			characterMoved();
		}
	}
}
void MainGame::keyRight()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(isTileWalkable(mainCharacter->x+1,mainCharacter->y) && !isTileOccupied(mainCharacter->x+1,mainCharacter->y) && currSpeed > 0)
		{
			mainCharacter->x += 1;
			characterMoved();
		}
	}
}

// handles mouse interactions
void MainGame::mouseLeft(int clickX, int clickY)
{
	if(exitBtn.inBounds(clickX, clickY))
	{
		signalCompletion();
	}
	else if(state == STATE_SHOP)
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
	else if(state == STATE_HUMAN_TURN)
	{
		if(clickedEnemy(clickX, clickY))
		{}
		else if(selectedEnemy != NULL && controlBtns[0].inBounds(clickX, clickY))
		{ // user clicked the "attack" button
			if(inRange(selectedEnemy,mainCharacter->getWeaponRange()) && !attackedThisRound)
			{
				fight(selectedEnemy);
				paintNow();
			}
		}
		else if(controlBtns[1].inBounds(clickX, clickY))
		{ // user clicked the "Equip Screen" button
			showEquipScreen();
		}
		else if(controlBtns[2].inBounds(clickX, clickY))
		{
			openTreasure();
		}
		else if(controlBtns[3].inBounds(clickX, clickY))
		{
			nextTurn();
		}
	}
	else
	{
		// checks if any of the map scrolling buttons were pressed
		for(int i=0;i<4;i++)
		{
			if(mapBtns[i].inBounds(clickX,clickY))
			{
				switch(i)
				{
					case 0: //left button
						gameMap.scrollLeft();
						break;
					case 1: //right button
						gameMap.scrollRight();
						break;
					case 2: //up button
						gameMap.scrollUp();
						break;
					case 3: //down button
						gameMap.scrollDown();
						break;							
				}
				
			}
		}
	}
}

void MainGame::mouseRight(int clickX, int clickY)
{
	if(state == STATE_SHOP)
		shopScreen->mouseRight(clickX,clickY);
	else if(state == STATE_EQUIP)
		equipScreen->mouseRight(clickX,clickY);
	else if(state == STATE_HUMAN_TURN)
	{
		if(selectedEnemy != NULL)
			unselectEnemy();
	}
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
	for(int i=0;i<numTreasure;i++)
	{
		if(inRange(treasure[i],1))
		{
			treasure[i]->obtainTreasure(mainCharacter);
			break;
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

//#######################################################################
//FIGHTING LOGIC
//#######################################################################
void MainGame::fight(Monster *thisMonster) 
{

	//MELEE ATTACK
	if (mainCharacter->getWeaponRange() == 1)
	{
		playerInitiativeRoll = mainCharacter->getInitiativeRoll();
		std::cout << "\n Your Initiative Roll: " << playerInitiativeRoll << "\n";
		monsterInitiativeRoll = thisMonster->getInitiativeRoll();
		std::cout << "Monsters Initiative Roll: " << monsterInitiativeRoll << "\n";

		if (playerInitiativeRoll >= monsterInitiativeRoll)
		{
			std::cout << "\n YOU ATTACK FIRST! \n";

			if (fPlayerAttackRoll(thisMonster))
				thisMonster->setHp( thisMonster->getHp() - rollDamageMelee() );

			if (fMonsterAttackRoll(thisMonster))
				mainCharacter->setHp( mainCharacter->getHp() - rollDamageMelee(thisMonster) );
		}

		else
		{
			std::cout << "\n MONSTER ATTACKS FIRST! \n";

			if (fMonsterAttackRoll(thisMonster))
				mainCharacter->setHp( mainCharacter->getHp() - rollDamageMelee(thisMonster) );

			if (fPlayerAttackRoll(thisMonster))
				thisMonster->setHp( thisMonster->getHp() - rollDamageMelee() );
		}
	}
	
	//RANGED ATTACK
	else
	{
	
		//check if you have enough of the consumable
		//and if you do, decrease it.


		switch (mainCharacter->equippedWeapon->requiredConsumable)
		{
		case UsableItem::ARROW:
			if (mainCharacter->getNumArrows() == 0)
			{
				std::cout << "You do not have enough arrows\n";
				damageRoll = false;
			}
			else
			{
				mainCharacter->setNumArrows( mainCharacter->getNumArrows() - 1 );
				damageRoll = true;
			}
			break;
		case UsableItem::BOLT:
			if (mainCharacter->getNumBolts() == 0)
			{
				std::cout << "You do not have enough bolts\n";
				damageRoll = false;
			}
			else
			{
				mainCharacter->setNumBolts( mainCharacter->getNumBolts() - 1 );
				damageRoll = true;
			}
			break;
		}

		if (damageRoll)
		std::cout << "\n YOU ATTACK WITH RANGED WEAPON! \n";

		if (fPlayerAttackRoll(thisMonster))
			thisMonster->setHp( thisMonster->getHp() - rollDamageMelee() );
	}
}

bool MainGame::fMonsterAttackRoll(Monster *thisMonster)
{
	monsterAttackRoll = Dice::roll(Dice::D20) + thisMonster->getAttackBonus();

	std::cout << "\nMonster Attack Roll: " << monsterAttackRoll << "\n";

	//critical hit implimentation
	if (monsterAttackRoll == 20) 
	{
		std::cout << "MONSTER AUTOMATICALY HITS YOU" << "\n\n";
		return true;
	}

	//normal hit
	if (monsterAttackRoll >= (thisMonster->getAC())) {
		std::cout << "MONSTER HITS YOU!" << "\n\n";
		return true;
	}

	//automatic miss
	if (monsterAttackRoll == 1) {
		std::cout << "MONSTER AUTOMATICALLY HITS YOU!" << "\n\n";
		return false;
	}
	return false;
}

bool MainGame::fPlayerAttackRoll(Monster *thisMonster)
{

	playerAttackRoll = Dice::roll(Dice::D20) + mainCharacter->getAttackBonus();

	std::cout << "\nYour Attack Roll: " << playerAttackRoll << "\n";

	//critical hit implimentation
	if (playerAttackRoll == 20) 
	{ 
		std::cout << "YOU AUTOMATICALY HIT HIM" << "\n\n";
		return true;
	}

	//Normal Hit
	if (playerAttackRoll >= (thisMonster->getAC())) 
	{
		std::cout << "YOU HIT HIM!" << "\n\n";
		return true;
	}

	//automatic miss
	if (playerAttackRoll == 1) 
	{
		std::cout << "YOU AUTOMATICALLY MISS HIM!" << "\n\n";
		return false;
	}

	return false;
}

int MainGame::rollDamageRanged()
{
	playerDamageRoll = Dice::roll(mainCharacter->equippedWeapon->getDamage()) + (mainCharacter->getDexMod());

	std::cout <<"\nYour Damage Roll: " << playerDamageRoll << "\n";

	if (playerDamageRoll <= 1) 
	{
		std::cout << "You deal an automatic 1 damage \n";
		return 1;
	}
	else
	{
		std::cout << "You deal " << playerDamageRoll << " damage points\n\n";
		return 0;
	}
	
	return -1;
}


int MainGame::rollDamageMelee()
{
	if (mainCharacter->equippedWeapon != NULL)
		playerDamageRoll = Dice::roll(mainCharacter->equippedWeapon->getDamage()) + (mainCharacter->getStrMod());
	else
		playerDamageRoll = Dice::roll(mainCharacter->HAND_COMBAT_DICE) + (mainCharacter->getStrMod());

	std::cout <<"\nYour Damage Roll: " << playerDamageRoll << "\n";

	if (playerDamageRoll <= 1) 
	{
		std::cout << "You deal an automatic 1 damage \n";
		return 1;
	}
	else
	{
		std::cout << "You deal " << playerDamageRoll << " damage points\n\n";
		return playerDamageRoll;
	}

	return -1;
}

int MainGame::rollDamageMelee(Monster *thisMonster) 
{
	monsterDamageRoll = Dice::roll(thisMonster->getDamageDiceType()) + (thisMonster->getStrMod());
	std::cout <<"\nMonster Damage Roll: " << monsterDamageRoll << "\n";

	if (monsterDamageRoll <= 1) 
	{
		std::cout << "Monster deals an automatic 1 damage \n";
		return 1;
	}
	else
	{
		std::cout << "Monster deals " << monsterDamageRoll << " damage points\n\n";
		return monsterDamageRoll;
	}

	return -1;
}

