#include "Graphics.h"

//Screen attributes 
const int SCREEN_WIDTH = 672; 
const int SCREEN_HEIGHT = 512; 
const int SCREEN_BPP = 32;
const char* WINDOW_TITLE = "COMP446 Assignment 1: Map Loader";

//The event structure that will be used 
SDL_Event event;

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;

// simple level object for illustrative purposes
// a more complete solution will be included in the final project
class Level
{
public:
	char *graphics;
	char *index;
	int alphaR;
	int alphaG;
	int alphaB;
};

// loads a game level
void initLevel00(Level *level)
{
	level->graphics = ".\\levels\\level00\\graphicTiles.bmp";
	level->index = ".\\levels\\level00\\index.map";
	level->alphaR = 0xFF;
	level->alphaG = 0xE2;
	level->alphaB = 0xAA;
}

int main( int argc, char* args[] ) 
{
	// Initialize the SDL library 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) 
	{ 
		return 1; 
	} 
	
	// Initialize the main graphics screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 

	// load and paint the background image
	background = loadImage(".\\levels\\level00\\background.png");
	applySurface(0,0,background,screen);

	// set the window title for the main window
	SDL_WM_SetCaption( WINDOW_TITLE, NULL );

	// create an on-screen map at (20px,20px) of size 160px by 160px
	Map gameMap(16,16,640,480);

	// create and initialize the first level
	std::cout << "*** Initializing Level 00 ***\n";
	Level currentLevel;
	initLevel00(&currentLevel);

	// initialize the graphics used to display the level
	Graphics graphics(currentLevel.graphics,currentLevel.alphaR,currentLevel.alphaG,currentLevel.alphaB);

	// load the level using its graphics into the on-screen map
	gameMap.loadMap(&graphics,currentLevel.index);

	// display the map (showing the first level)
	gameMap.paint();

	//Update the screen 
	if( SDL_Flip( screen ) == -1 ) 
	{ 
		return 4; 
	}

	std::cout << "\nUse the keyboard arrows to look around on the map\n";

	//Make sure the program waits for a quit, instead of a timed exit
	bool quit = false;


		//While the user hasn't quit 
	// Infinite loop for user input... chews CPU
	while(quit != true) 
	{
		//While there's an event to handle 
		while( SDL_PollEvent( &event ) ) 
		{
			//If a key was pressed 
			if( event.type == SDL_KEYDOWN ) 
			{
				switch( event.key.keysym.sym ) 
				{ 
					case SDLK_UP:
						gameMap.scrollUp();
						break; 
					case SDLK_DOWN:
						gameMap.scrollDown();
						break; 
					case SDLK_LEFT:
						gameMap.scrollLeft();
						break; 
					case SDLK_RIGHT:
						gameMap.scrollRight();
						break; 
				}

				// reapply background and paint tiles again
				applySurface(0,0,background,screen);
				gameMap.paint();
			}
			//If the user has Xed out the window 
			if( event.type == SDL_QUIT ) 
			{ 
				//Quit the program 
				quit = true; 
			} 
		}

		//Update the screen 
		if( SDL_Flip( screen ) == -1 ) 
		{ 
			return 4; 
		}

		// to avoid chewing too much CPU
		SDL_Delay(20);
	}


	return 0;
}