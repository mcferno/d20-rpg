#include "Graphics.h"
#include "SDL_ttf.h"

// Screen attributes 
const int SCREEN_WIDTH = 896; 
const int SCREEN_HEIGHT = 768; 
const int SCREEN_BPP = 32;
const char* WINDOW_TITLE = "COMP446 Assignment 1: Map Editor";
const int FRAME_RATE_SLEEP = 40;

// location to save the user's map
char* MAP = ".\\index.map";

// the event structure that will be used 
SDL_Event event;

SDL_Surface *screen = NULL;
TTF_Font *font = NULL; 

// the font color used for on screen text 
SDL_Color textColor = { 255, 255, 255 };

/* Class EditableMap: extension of the Map class which allows the user to make
 *   changes to the map which has been loaded. Adds the ability to save the map
 *   and remove tile layers (erase).
 */
class EditableMap : public Map
{
private:
	// empty graphic tile to show a tile without assigned graphics
	SDL_Surface *emptyTile;

	void init()
	{
		emptyTile = loadImage(".\\images\\emptycell.png");

		if(emptyTile == NULL)
			std::cout << "Error loading empty tile!\n";
	}

public:
	EditableMap()
	{}

	// map is created, passing the specs of its bounding window
	EditableMap(int newX, int newY, int newW, int newH): Map(newX,newY,newW,newH)
	{
		init();
	}

	// creates an empty map with the sizes provided
	void loadEmptyMap(int newW, int newH)
	{
		w = newW;
		h = newH;

		if(limitWTiles > w)
			limitWTiles = w;

		if(limitHTiles > h)
			limitHTiles = h;

		// initialize 2D array
		ts = new Tile*[w];
		for (int i = 0; i < w; i++)
			ts[i] = new Tile[h];

		// set all the tiles as empty
		for(i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				ts[i][j].b = -1;
				ts[i][j].f = -1;
				ts[i][j].isWalkable = true;
			}
		}
	}

	// paints the entire map
	void paint()
	{
		for(int i=x;i<x+limitWTiles;i++)
		{
			for(int j=y;j<y+limitHTiles;j++)
			{
				paintTile(i,j);
			}
		}
	}

	// paints a specific tile, using the empty tile if it hasn't been assigned graphics
	void paintTile(int indexX, int indexY, int locX = -1, int locY = -1)
	{
		// paint elsewhere than the default location?
		if(locX == -1)
			locX = limit.x+(indexX-x)*tileSize;
		if(locY == -1)
			locY = limit.y+(indexY-y)*tileSize;

		if(ts[indexX][indexY].b == -1)
			applySurface(locX,locY,emptyTile,screen);
		else
			applySurface(locX,locY,g->image,screen,&g->clip[ts[indexX][indexY].b]);
		if(ts[indexX][indexY].f != -1)
			applySurface(locX,locY,g->image,screen,&g->clip[ts[indexX][indexY].f]);
	}

	// saves the currently loaded map to a file
	void save(char *filename)
	{
		std::ofstream mapFile(filename);

		std::cout << "Saving the map index \"" << filename << "\" ... ";

		// verify that the file was opened correctly before reading 
		if(mapFile == NULL) 
		{ 
			std::cout << "FAILED!\n";
			return; 
		}

		// sets the width and height of the map
		mapFile << w << "\n";
		mapFile << h << "\n";

		// write the information about every tile on the map
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				mapFile << ts[i][j].b << " " << ts[i][j].f << " " << ((ts[i][j].isWalkable)?"Y":"N") << "\n";
			}
		}
		mapFile.close();
		std::cout << "Successful!\n";
	}
};

/* Class SelectableMap: extension and simplification of the Map class which 
 *   sets its tiles according to a graphic image instead of loading an index
 *   file.
 */
class SelectableMap : public Map
{
private:
	// the image used to show which graphic tile is currently selected.
	SDL_Surface *highlightTile;
public:
	SelectableMap()
	{
	}
	// map is created, passing the specs of its bounding window
	SelectableMap(int newX, int newY, int newW, int newH): Map(newX,newY,newW,newH)
	{
		highlightTile = loadImage(".\\images\\highlight.png",0x0,0xFF,0xFF);

		if(highlightTile == NULL)
			std::cout << "Highlight Tile Error'ed\n";
	}

	// paints all the graphic tiles, including the highlighting of the selected tile
	void paint(int highlightTileX = -1, int highlightTileY = -1)
	{
		SDL_Rect windowClip;

		windowClip.x = x*tileSize;
		windowClip.y = y*tileSize;
		windowClip.w = limit.w;
		windowClip.h = limit.h;
		
		applySurface(limit.x,limit.y,g->image,screen,&windowClip);

		if(highlightTileX != -1 && highlightTileY != -1 && isOnScreen(highlightTileX,highlightTileY))
		{
			applySurface(limit.x+(highlightTileX-x)*tileSize,limit.y+(highlightTileY-y)*tileSize,highlightTile,screen);
		}
	}

	void loadGraphics(Graphics *newGraphics)
	{
		Map::loadGraphics(newGraphics);
		w = g->w/tileSize;
		h = g->h/tileSize;
	}
};

// simple class representing a clickable button
class Button : public Rect
{
public:
	// the image representing the button
	SDL_Surface* image;

	Button(int newX = -1, int newY = -1, int newW = -1, int newH = -1)
	{
		x = newX;
		y = newY;
		w = newW;
		h = newH;
	}

	void loadImg(char *filename, int r = -1, int g = -1, int b = -1)
	{
		image = loadImage(filename,r,g,b);
	}
	
};

/* Class MapEditor: Uses the Map class and its child classes to make a map editor.
 *   Allows maps to be loaded, edited and saved.
 */
class MapEditor
{
private:
	// tracks the indexes of selected map cells and graphic tiles
	int selectedTileX, selectedTileY, selectedTileIndex, selectedCellX, selectedCellY;
	Tile *selectedCell;

	SDL_Surface *background;
	SDL_Surface *selectedTileMsg, *selectedCellMsg, *xCoordLabel, *yCoordLabel, *foregroundMsg, *backgroundMsg, *indexMsg, *walkableMsg;

	Graphics *graphics;

	EditableMap customMap;
	Button customMapBtns[4];

	SelectableMap tileSet;
	Button tileSetBtns[4];

	int tileSize;

	// checks if an x,y coordinate is within the bounds of a certain screen area
	bool inBounds(int x, int y, Rect limits)
	{
		return (x >= limits.x && 
				x <= limits.w + limits.x &&
				y >= limits.y && 
				y <= limits.h + limits.y);
	}

	// converts an x,y coordinate in pixels into a 2-d array index for the corresponding tile
	void coordToIndex(int *newX, int *newY, Map mapPt)
	{
		*newX -= mapPt.limit.x;
		*newX /= tileSize;
		*newX += mapPt.x;
		*newY -= mapPt.limit.y;
		*newY /= tileSize;
		*newY += mapPt.y;
	}

	// converts an x,y coordinate (in tiles) into a 1-d array index
	int getTileArrayIndex(int x, int y)
	{
		return (tileSet.w * y + x);
	}

	// if a custom map cell is selected, switch between walkable and non-walkable
	void toggleWalkable()
	{
		if(selectedCell != NULL)
		{
			selectedCell->isWalkable = !selectedCell->isWalkable;
		}
	}

	// checks if the scroll buttons were pressed, if so, it invokes the appropriate scrolling
	void checkButtons(int x, int y, Button *setOfButtons, Map *mapToAffect)
	{
		for(int i=0;i<4;i++)
		{
			if(inBounds(x,y,setOfButtons[i]))
			{
				switch(i)
				{
					case 0: //left button
						mapToAffect->scrollLeft();
						break;
					case 1: //right button
						mapToAffect->scrollRight();
						break;
					case 2: //up button
						mapToAffect->scrollUp();
						break;
					case 3: //down button
						mapToAffect->scrollDown();
						break;							
				}
				
			}
		}
	}

public:
	MapEditor()
	{
		std::cout << "\n" << WINDOW_TITLE << "\n\n"
				  << "INITIALIZING...\n";
		selectedTileX = selectedTileY = selectedCellX = selectedCellY = selectedTileIndex = -1;
		background = loadImage(".\\images\\mapEditorBg.png");

		// graphics to be used for the creation of a map
		graphics = new Graphics(".\\images\\defaultTiles.bmp",0xFF,0xE2,0xAA);
		tileSize = graphics->tileSize;

		// create a new empty map
		customMap = EditableMap(16,16,640,480);
		customMap.loadGraphics(graphics);
		customMap.loadEmptyMap(42,32);

		// code to load an existing map
		//customMap.loadMap(graphics,MAP);
		// - or -
		//customMap.loadGraphics(graphics);
		//customMap.parseIndex(MAP);

		// initialize the 4 directional scrolling arrows for the custom map
		customMapBtns[0] = Button(16,496,16,16);
		customMapBtns[0].loadImg(".\\images\\leftArrow.png",0xFF,0x0,0xFF);
		customMapBtns[1] = Button(640,496,16,16);
		customMapBtns[1].loadImg(".\\images\\rightArrow.png",0xFF,0x0,0xFF);
		customMapBtns[2] = Button(656,16,16,16); 
		customMapBtns[2].loadImg(".\\images\\upArrow.png",0xFF,0x0,0xFF);
		customMapBtns[3] = Button(656,480,16,16);
		customMapBtns[3].loadImg(".\\images\\downArrow.png",0xFF,0x0,0xFF);

		// create and load the graphic tiles used to make the custom map with
		std::cout << "Creating the graphic tiles and window ... ";
		tileSet = SelectableMap(16,528,640,224);
		tileSet.loadGraphics(graphics);
		std::cout << "Successful!\n";

		// initialize the 4 directional scrolling arrows for the graphic tiles
		tileSetBtns[0] = Button(16,752,16,16);
		tileSetBtns[0].loadImg(".\\images\\leftArrow.png",0xFF,0x0,0xFF);
		tileSetBtns[1] = Button(640,752,16,16);
		tileSetBtns[1].loadImg(".\\images\\rightArrow.png",0xFF,0x0,0xFF);
		tileSetBtns[2] = Button(656,528,16,16); 
		tileSetBtns[2].loadImg(".\\images\\upArrow.png",0xFF,0x0,0xFF);
		tileSetBtns[3] = Button(656,736,16,16);
		tileSetBtns[3].loadImg(".\\images\\downArrow.png",0xFF,0x0,0xFF);

		std::cout << "Rendering all text displays ... ";
		selectedTileMsg = TTF_RenderText_Solid( font, "Selected Tile:", textColor );
		selectedCellMsg = TTF_RenderText_Solid( font, "Selected Map Cell:", textColor );
		xCoordLabel = TTF_RenderText_Solid( font, "x:", textColor );
		yCoordLabel = TTF_RenderText_Solid( font, "y:", textColor );
		foregroundMsg = TTF_RenderText_Solid( font, "Foreground:", textColor );
		backgroundMsg = TTF_RenderText_Solid( font, "Background:", textColor );
		indexMsg = TTF_RenderText_Solid( font, "Index:", textColor );
		walkableMsg = TTF_RenderText_Solid( font, "Walkable:", textColor );
		std::cout << "Successful!\n";
	}

	~MapEditor()
	{
		SDL_FreeSurface(background);
		SDL_FreeSurface(selectedTileMsg);
		SDL_FreeSurface(selectedCellMsg);
		SDL_FreeSurface(xCoordLabel);
		SDL_FreeSurface(yCoordLabel);
		SDL_FreeSurface(foregroundMsg);
		SDL_FreeSurface(backgroundMsg);
		SDL_FreeSurface(indexMsg);
		SDL_FreeSurface(walkableMsg);
	}

	void paintInfoPanel()
	{
		// temporary buffer for on-the-fly conversion from int to char string
		char tempBuffer[4];

		// simple coordinates to inform the user how much they are scrolling on the main map
		applySurface(16,0,TTF_RenderText_Solid( font, itoa(customMap.x,tempBuffer,10), textColor),screen);
		applySurface(48,0,TTF_RenderText_Solid( font, itoa(customMap.y,tempBuffer,10), textColor),screen);

		applySurface(688,16,selectedCellMsg,screen);
		applySurface(688,32,xCoordLabel,screen);
		applySurface(736,32,yCoordLabel,screen);
		applySurface(688,48,foregroundMsg,screen);
		applySurface(688,64,backgroundMsg,screen);
		applySurface(688,80,walkableMsg,screen);

		// renders specifics of the selected custom map cell on-the-fly since they change frequently
		if(selectedCellX != -1 && selectedCellY != -1)
		{
			applySurface(704,32,TTF_RenderText_Solid( font, itoa(selectedCellX,tempBuffer,10), textColor),screen);
			applySurface(752,32,TTF_RenderText_Solid( font, itoa(selectedCellY,tempBuffer,10), textColor),screen);

			// paint information about the background layer if it is initialized
			if(selectedCell->b != -1)
			{
				applySurface(768,64,TTF_RenderText_Solid( font, itoa(selectedCell->b,tempBuffer,10), textColor),screen);
				applySurface(768,80,TTF_RenderText_Solid( font, (selectedCell->isWalkable)?"Y":"N", textColor),screen);
			}

			// paint information about the foreground layer if it is initialized
			if(selectedCell->f != -1)
				applySurface(768,48,TTF_RenderText_Solid( font, itoa(selectedCell->f,tempBuffer,10), textColor),screen);

			// paint the selected cell itself
			customMap.paintTile(selectedCellX,selectedCellY,800,16);
		}

		applySurface(688,112,selectedTileMsg,screen);
		applySurface(688,128,indexMsg,screen);

		// paint information about the selected graphics
		if(selectedTileIndex != -1)
		{
			applySurface(736,128,TTF_RenderText_Solid( font, itoa(selectedTileIndex,tempBuffer,10), textColor),screen);
			applySurface(800,112,graphics->image,screen,&graphics->clip[selectedTileIndex]);
		}
	}

	// full layered paint of the map editor and all of its inner sections
	void paint()
	{
		applySurface(0,0,background,screen);

		customMap.paint();
		tileSet.paint(selectedTileX,selectedTileY);

		// paint the 4 directional buttons for each scrolling window
		for(int i=0;i<4;i++)
		{
			applySurface(customMapBtns[i].x,customMapBtns[i].y,customMapBtns[i].image,screen);
			applySurface(tileSetBtns[i].x,tileSetBtns[i].y,tileSetBtns[i].image,screen);
		}

		paintInfoPanel();
	}

	void mouseLeft(int x, int y) 
	{
		// check if the custom map was clicked
		if(inBounds(x,y,customMap.limit))
		{
			coordToIndex(&x,&y,customMap);
			selectedCellX = x;
			selectedCellY = y;
			selectedCell = customMap.getTile(selectedCellX,selectedCellY);

			if(selectedCell == NULL)
			{
				selectedCellX = -1;
				selectedCellY = -1;
			}
			else
			{
				// copy a graphic tile to the background or foreground of a cell
				if(selectedTileIndex != -1)
				{
					if(selectedCell->b == -1)
						selectedCell->b = selectedTileIndex;
					else
						if(selectedCell->b != selectedTileIndex)
							selectedCell->f = selectedTileIndex;
				}
			}
		}
		// check if a graphic tile was clicked, if so select it for copying
		else if(inBounds(x,y,tileSet.limit))
		{
			coordToIndex(&x,&y,tileSet);
			selectedTileX = x;
			selectedTileY = y;
			selectedTileIndex = getTileArrayIndex(x,y);
		}
		// check if the scrolling arrows were pressed
		else
		{
			checkButtons(x,y,customMapBtns,&customMap);
			checkButtons(x,y,tileSetBtns,&tileSet);
		}
	}

	void mouseRight(int x, int y)
	{
		// deselect a graphic tile if one is selected
		if(selectedTileX != -1 || selectedTileY != -1 || selectedTileIndex != -1)
		{
			selectedTileX = -1;
			selectedTileY = -1;
			selectedTileIndex = -1;
		}
		// deselect a custom map tile if one is selected
		else if(selectedCell != NULL || selectedCellX != -1 || selectedCellY != -1)
		{
			selectedCell = NULL;
			selectedCellX = -1;
			selectedCellY = -1;
		}
		// erase a layer of a custom map tile.
		else if(inBounds(x,y,customMap.limit))
		{
			coordToIndex(&x,&y,customMap);
			Tile *tempCell = customMap.getTile(x,y);
			if(tempCell != NULL)
			{
				if(tempCell->f != -1)
					tempCell->f = -1;
				else
					tempCell->b = -1;
			}
		}
		else
		{
			// no default action needed
		}

	}

	void showInstructions()
	{
		std::cout << "\nINSTRUCTIONS:\n"
				  << "When a graphic tile is selected:\n"
				  << " - Click the custom map with the left button to place a tile\n"
				  << "   - Place 2 graphics tiles on the same cell to layer them.\n"
				  << "When a custom map cell is selected:\n"
				  << " - Press 'w' to set the tile 'Walkable' or 'Non-Walkable'\n\n"
				  << "Right-click any time to unselected the cell or graphic tile\n"
				  << "Right-click a custom map cell to remove its fore/background layer\n"
				  << "Press 's' to save the current map to a file\n";
	}

	// main function which controls most of the user interaction
	void run()
	{
		showInstructions();

		// paint for the first time
		paint();

		//Make sure the program waits for a quit, instead of a timed exit
		bool quit = false;

		//While the user hasn't quit 
		while(quit != true) 
		{
			//While there's an event (keypress, mouse, etc) to handle 
			while( SDL_PollEvent( &event ) ) 
			{
				// If a key was pressed 
				if( event.type == SDL_KEYDOWN ) 
				{
					switch( event.key.keysym.sym ) 
					{
						// tile walkable toggling, triggered by the 'w' key.
						case SDLK_w:
							toggleWalkable();
							break; 
						// save functionality, triggered by the 's' key.
						case SDLK_s:
							customMap.save(MAP);
							break; 
					}
					paint();
				}
				
				// If a mouse button was pressed
				if( event.type == SDL_MOUSEBUTTONDOWN )
				{
					if( event.button.button == SDL_BUTTON_LEFT )
					{
						mouseLeft(event.button.x,event.button.y);
						paint();
					}
					if( event.button.button == SDL_BUTTON_RIGHT )
					{
						mouseRight(event.button.x,event.button.y);
						paint();
					}
				}

				// If the user has pressed the window's close button
				if( event.type == SDL_QUIT ) 
				{ 
					quit = true; 
				} 
			}

			// Update the screen to reflect any changes that may have occured
			if( SDL_Flip( screen ) == -1 ) 
			{ 
				return; 
			}

			// Short sleep to avoid chewing too much CPU while waiting for user input
			SDL_Delay(FRAME_RATE_SLEEP);
		}
	}

};

int main( int argc, char* args[] ) 
{
	// Initialize the SDL library 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) 
	{ 
		return 1; 
	} 

	//Initialize SDL_ttf, which handles font rendering
	if( TTF_Init() == -1 ) 
	{
		return 1; 
	}
	
	// Initialize the main graphics screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 

	// set the window title for the main window
	SDL_WM_SetCaption( WINDOW_TITLE, NULL );

	font = TTF_OpenFont( ".\\fonts\\tahoma.ttf", 12 );

	MapEditor mapEditor;

	// the map editor will handle the rest of the user interaction
	mapEditor.run();
	return 0;
}