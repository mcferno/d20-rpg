#include "startScreen.h"


StartScreen::StartScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
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

void StartScreen::setSignalS(bool* signal)
{
	signalCompletionS = signal;
}

void StartScreen::paint()
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

void StartScreen::mouseLeft(int x, int y)
{

	if (x >= 0 && x <= 800 && y >= 0 && y <= 600)
	{
		Mix_HaltMusic();
		*signalCompletionS = true;
	}
}

void StartScreen::mouseRight(int x, int y)
{

	if (x >= 0 && x <= 800 && y >= 0 && y <= 600)
	{
		Mix_HaltMusic();
		*signalCompletionS = true;
	}
}

bool StartScreen::inBounds(GraphicsSelection &gs, int x, int y)
{
	return (x >= gs.x && x <= (gs.x + gs.clip.w) && y >= gs.y && y <= (gs.y + gs.clip.h));
}

