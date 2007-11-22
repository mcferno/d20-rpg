#include "startScreen.h"


StartScreen::StartScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	mapEditor = NULL;
	state = STATE_SHOW_SCREEN;
	isDone = false;
	init();
}

void StartScreen::init()
{

	startMusic = Mix_LoadMUS(".\\music\\start.mp3");
	musicStarted = false;

	startScreen0 = loadImage(".\\images\\anim\\Image0.png");
	startScreen1 = loadImage(".\\images\\anim\\Image1.png");
	startScreen2 = loadImage(".\\images\\anim\\Image2.png");
	startScreen3 = loadImage(".\\images\\anim\\Image3.png");
	startScreen4 = loadImage(".\\images\\anim\\Image4.png");
	startScreen5 = loadImage(".\\images\\anim\\Image5.png");
	startScreen6 = loadImage(".\\images\\anim\\Image6.png");
}

void StartScreen::paint()
{
	if(state == STATE_SHOW_SCREEN)
	{
		//start music
		if (!musicStarted)
		{
			Mix_PlayMusic( startMusic,-1 );
			musicStarted=true;
		}

		// apply background image
		applySurface(0,0,startScreen6,screen);
	}
	else if(state == STATE_MAP_EDITOR)
	{
		mapEditor->paint();
	}
}

void StartScreen::mouseLeft(int clickX, int clickY)
{
	if(state == STATE_SHOW_SCREEN)
	{
		if (clickX >= x && clickX <= x+w && clickY >= y && clickY <= y+h)
		{
			Mix_HaltMusic();
			signalCompletion();
		}
	}
	else if(state == STATE_MAP_EDITOR)
	{
		mapEditor->mouseLeft(clickX, clickY);
	}
}

void StartScreen::mouseRight(int clickX, int clickY)
{
	if(state == STATE_SHOW_SCREEN)
	{
		if (clickX >= x && clickX <= x+w && clickY >= y && clickY <= y+h)
		{
			Mix_HaltMusic();
			signalCompletion();
		}
	}
	else if(state == STATE_MAP_EDITOR)
	{
		mapEditor->mouseRight(clickX, clickY);
	}
}

bool StartScreen::inBounds(GraphicsSelection &gs, int x, int y)
{
	return (x >= gs.x && x <= (gs.x + gs.clip.w) && y >= gs.y && y <= (gs.y + gs.clip.h));
}

void StartScreen::showMapEditor(int level)
{
	if(state != STATE_MAP_EDITOR)
	{
		if(mapEditor == NULL)
		{
			mapEditor = new MapEditor(x,y,w,h);
			mapEditor->setSignal(&isDone);
		}
		switch(level)
		{
			case 1:
				mapEditor->load(LEVEL_1_MAP, LEVEL_1_GRAPHICS);
				break;
			case 2:
				mapEditor->load(LEVEL_2_MAP, LEVEL_2_GRAPHICS);
				break;
			default:
				mapEditor->load();
				break;
		}
		state = STATE_MAP_EDITOR;
	}
}

