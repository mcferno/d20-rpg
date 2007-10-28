#include "Screens.h"

#include <iostream>

Graphics *characterSprites = NULL;
Graphics *selectScreen = NULL;
Character *mainCharacter = NULL;

void SelectionScreen::init()
{
	characterSprites = new Graphics(".\\images\\characters.bmp");
	selectScreen = new Graphics(".\\images\\selectScreen.png");

}

SelectionScreen::SelectionScreen()
{
}

SelectionScreen::SelectionScreen(SDL_Surface *newScreen)
{
	screen = newScreen;
	init();
}

void SelectionScreen::paint()
{

	// paint background
	// if player graphic selected? paint in selected character area
	// if race selected? 


	applySurface(0,0,selectScreen->image,screen);
}

void SelectionScreen::mouseLeft(int x, int y)
{
	if (x >= 672 && x <= 784 && y >= 544 && y <= 591)
	{
		
		//state = STATE_MAIN_GAME;
		//mainGame.init();
	}
}

void SelectionScreen::mouseRight(int x, int y)
{
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const int MainGame::LEVEL_1 = 1;
const int MainGame::STATE_LEVEL_START = 0;
const int MainGame::STATE_HUMAN_TURN = 1;
const int MainGame::STATE_AI_TURN = 2;

MainGame::MainGame()
{
	std::cout << "called";
}

MainGame::MainGame(SDL_Surface *newScreen)
{
	screen = newScreen;
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,640,480);

	std::cout << gameMap.limit.x << "----\n";
}

void MainGame::init()
{
	loadLevel();
	gameMap.loadGraphics(level->graphics, level->alphaR, level->alphaG, level->alphaB);
	gameMap.parseIndex(level->index);
	state = STATE_LEVEL_START;

	// hacked in, fake character
	mainCharacter = new Fighter(HUMAN);
	mainCharacter->graphics = characterSprites;
	SDL_Rect *tempRect = new SDL_Rect();

	tempRect->x = 0;
	tempRect->y = 0;
	tempRect->w = 16;
	tempRect->h = 16;

	mainCharacter->x = 10;
	mainCharacter->y = 14;

	mainCharacter->clip = tempRect;
}

void MainGame::loadLevel()
{
	switch(currentLevel)
	{
		case LEVEL_1:
			level = new Level();
			level->graphics = ".\\levels\\level00\\graphicTiles.bmp";
			level->index = ".\\levels\\level00\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;
			break;
	}
}

void MainGame::paint()
{
	gameMap.paint();

	if(gameMap.isOnScreen(mainCharacter->x,mainCharacter->y))
	{
		paintCharacter(mainCharacter);
	}
}

void MainGame::paintCharacter(Character* c)
{
	int x = gameMap.limit.x + (c->x - gameMap.x)*16;
	int y = gameMap.limit.y + (c->y - gameMap.y)*16;
	applySurface(x, y, c->graphics->image, screen, c->clip);
}

void MainGame::keyUp()
{
	if(mainCharacter->y > 0)
	{
		if(gameMap.ts[mainCharacter->x][mainCharacter->y-1].isWalkable)
			mainCharacter->y -= 1;
	}
}
void MainGame::keyDown()
{
	if(mainCharacter->y < gameMap.h)
	{
		if(gameMap.ts[mainCharacter->x][mainCharacter->y+1].isWalkable)
			mainCharacter->y += 1;
	}
}
void MainGame::keyLeft()
{
	if(mainCharacter->x > 0)
	{
		if(gameMap.ts[mainCharacter->x-1][mainCharacter->y].isWalkable)
			mainCharacter->x -= 1;
	}
}
void MainGame::keyRight()
{
	if(mainCharacter->x < gameMap.w)
	{
		if(gameMap.ts[mainCharacter->x+1][mainCharacter->y].isWalkable)
			mainCharacter->x += 1;
	}
}