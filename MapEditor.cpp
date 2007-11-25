
#include "MapEditor.h"

// the font color used for on screen text 
SDL_Color textColor = { 255, 255, 255 };

// ###########################################################################################

void EditableMap::init()
{
	emptyTile = loadImage(".\\images\\emptycell.png");

	if(emptyTile == NULL)
		std::cout << "Error loading empty tile!\n";
}

EditableMap::EditableMap()
{}

// map is created, passing the specs of its bounding window
EditableMap::EditableMap(int newX, int newY, int newW, int newH): Map(newX,newY,newW,newH)
{
	init();
}

// creates an empty map with the sizes provided
void EditableMap::loadEmptyMap(int newW, int newH)
{
	w = newW;
	h = newH;

	if(limitWTiles > w)
		limitWTiles = w;

	if(limitHTiles > h)
		limitHTiles = h;

	// initialize 2D array
	ts = new Tile*[w];
	for (int k = 0; k < w; k++)
		ts[k] = new Tile[h];

	// set all the tiles as empty
	for(int i=0;i<w;i++)
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
void EditableMap::paint()
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
void EditableMap::paintTile(int indexX, int indexY, int locX, int locY)
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
void EditableMap::save(char *filename)
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


// ###########################################################################################

/* Class SelectableMap: extension and simplification of the Map class which 
 *   sets its tiles according to a graphic image instead of loading an index
 *   file.
 */

SelectableMap::SelectableMap()
{
}
// map is created, passing the specs of its bounding window
SelectableMap::SelectableMap(int newX, int newY, int newW, int newH): Map(newX,newY,newW,newH)
{
	highlightTile = loadImage(".\\images\\highlight.png",0x0,0xFF,0xFF);

	if(highlightTile == NULL)
		std::cout << "Highlight Tile Error'ed\n";
}

// paints all the graphic tiles, including the highlighting of the selected tile
void SelectableMap::paint(int highlightTileX, int highlightTileY)
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

void SelectableMap::loadGraphics(Graphics *newGraphics)
{
	Map::loadGraphics(newGraphics);
	w = g->w/tileSize;
	h = g->h/tileSize;
}

// ###########################################################################################

/* Class MapEditor: Uses the Map class and its child classes to make a map editor.
 *   Allows maps to be loaded, edited and saved.
 */

const char* MapEditor::DEFAULT_TILES = ".\\images\\defaultTiles.bmp";
const char* MapEditor::DEFAULT_MAP_LOCATION = "index.map";

// checks if an x,y coordinate is within the bounds of a certain screen area
bool MapEditor::inBounds(int x, int y, Rect limits)
{
	return (x >= limits.x && 
			x <= limits.w + limits.x &&
			y >= limits.y && 
			y <= limits.h + limits.y);
}

// converts an x,y coordinate in pixels into a 2-d array index for the corresponding tile
void MapEditor::coordToIndex(int *newX, int *newY, Map *mapPt)
{
	*newX -= mapPt->limit.x;
	*newX /= tileSize;
	*newX += mapPt->x;
	*newY -= mapPt->limit.y;
	*newY /= tileSize;
	*newY += mapPt->y;
}

// converts an x,y coordinate (in tiles) into a 1-d array index
int MapEditor::getTileArrayIndex(int x, int y)
{
	return (tileSet->w * y + x);
}

// if a custom map cell is selected, switch between walkable and non-walkable
void MapEditor::toggleWalkable()
{
	if(selectedCell != NULL)
	{
		selectedCell->isWalkable = !selectedCell->isWalkable;
	}
}

// checks if the scroll buttons were pressed, if so, it invokes the appropriate scrolling
void MapEditor::checkButtons(int x, int y, Button *setOfButtons, Map *mapToAffect)
{
	for(int i=0;i<4;i++)
	{
		//if(inBounds(x,y,setOfButtons[i]))
		if(setOfButtons[i].inBounds(x,y))
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

MapEditor::MapEditor(int newX, int newY, int width, int height) : 
	Screen(newX, newY, width, height)
{
	selectedTileX = selectedTileY = selectedCellX = selectedCellY = selectedTileIndex = -1;
	background = loadImage(".\\images\\mapEditorBg.png");

	arrows = loadImage(".\\images\\arrows.png",0xFF,0x0,0xFF);

	// initialize the 4 directional scrolling arrows for the custom map
	//left
	customMapBtns[0] = Button(TILE_SIZE,24*TILE_SIZE,TILE_SIZE,TILE_SIZE,0,0,arrows);
	//right
	customMapBtns[1] = Button(37*TILE_SIZE,24*TILE_SIZE,TILE_SIZE,TILE_SIZE,TILE_SIZE,0,arrows);
	//up
	customMapBtns[2] = Button(38*TILE_SIZE,TILE_SIZE,TILE_SIZE,TILE_SIZE,2*TILE_SIZE,0,arrows);
	//down
	customMapBtns[3] = Button(38*TILE_SIZE,23*TILE_SIZE,TILE_SIZE,TILE_SIZE,3*TILE_SIZE,0,arrows);

	// initialize the 4 directional scrolling arrows for the graphic tiles
	//left
	tileSetBtns[0] = Button(TILE_SIZE,36*TILE_SIZE,TILE_SIZE,TILE_SIZE,0,0,arrows);
	//right
	tileSetBtns[1] = Button(48*TILE_SIZE,36*TILE_SIZE,TILE_SIZE,TILE_SIZE,TILE_SIZE,0,arrows);
	//up
	tileSetBtns[2] = Button(49*TILE_SIZE,26*TILE_SIZE,TILE_SIZE,TILE_SIZE,2*TILE_SIZE,0,arrows);
	//down
	tileSetBtns[3] = Button(49*TILE_SIZE,35*TILE_SIZE,TILE_SIZE,TILE_SIZE,3*TILE_SIZE,0,arrows);

	closeBtn = Rect(672,336,80,32);
	saveBtn = Rect(672,288,80,32);

	std::cout << "Rendering all text displays ... ";
	selectedTileMsg = TTF_RenderText_Solid( fontCalibriTiny, "Selected Tile:", textColor );
	selectedCellMsg = TTF_RenderText_Solid( fontCalibriTiny, "Selected Map Cell:", textColor );
	xCoordLabel = TTF_RenderText_Solid( fontCalibriTiny, "x:", textColor );
	yCoordLabel = TTF_RenderText_Solid( fontCalibriTiny, "y:", textColor );
	foregroundMsg = TTF_RenderText_Solid( fontCalibriTiny, "Foreground:", textColor );
	backgroundMsg = TTF_RenderText_Solid( fontCalibriTiny, "Background:", textColor );
	indexMsg = TTF_RenderText_Solid( fontCalibriTiny, "Index:", textColor );
	walkableMsg = TTF_RenderText_Solid( fontCalibriTiny, "Walkable:", textColor );
	std::cout << "Successful!\n";

	customMap = NULL;
	tileSet = NULL;
	graphics = NULL;
	currentMapFilename = NULL;

	isLoaded = false;
}

MapEditor::~MapEditor()
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

void MapEditor::load(const char* mapFile, const char* graphicsFile, int newMapW, int newMapH)
{
	// clear an old map/tileset if it exists
	if(customMap != NULL || tileSet != NULL || graphics != NULL)
		unload();

	customMap = new EditableMap(16,16,592,368);
	tileSet = new SelectableMap(16,416,768,160);

	if(mapFile != NULL && graphicsFile != NULL)
	{
		// graphics to be used for the creation of a map
		graphics = new Graphics(graphicsFile,DEFAULT_TILES_R,DEFAULT_TILES_G,DEFAULT_TILES_B);
		tileSize = graphics->tileSize;

		currentMapFilename = const_cast<char*>(mapFile);

		customMap->loadMap(graphics,mapFile);
	}
	else // load an empty map with the default tiles
	{
		// graphics to be used for the creation of a map
		graphics = new Graphics(DEFAULT_TILES,DEFAULT_TILES_R,DEFAULT_TILES_G,DEFAULT_TILES_B);
		tileSize = graphics->tileSize;

		currentMapFilename = NULL;

		// create a new empty map
		customMap->loadGraphics(graphics);
		customMap->loadEmptyMap(newMapW,newMapH);
	}

	tileSet->loadGraphics(graphics);

	selectedTileX = selectedTileY = selectedCellX = selectedCellY = selectedTileIndex = -1;
	isLoaded = true;
}

void MapEditor::unload()
{
	tileSize = 0;
	if(customMap != NULL)
		customMap = NULL;

	if(tileSet != NULL)
		tileSet = NULL;

	if(graphics != NULL)
		graphics = NULL;
}

void MapEditor::paintInfoPanel()
{
	// temporary buffer for on-the-fly conversion from int to char string
	char tempBuffer[4];

	// simple coordinates to inform the user how much they are scrolling on the main map
	_itoa_s(customMap->x,tempBuffer,10);
	applySurface(16,0,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);
	_itoa_s(customMap->y,tempBuffer,10);
	applySurface(48,0,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);

	applySurface(640,16,selectedCellMsg,screen);
	applySurface(640,32,xCoordLabel,screen);
	applySurface(688,32,yCoordLabel,screen);
	applySurface(640,48,foregroundMsg,screen);
	applySurface(640,64,backgroundMsg,screen);
	applySurface(640,80,walkableMsg,screen);

	// renders specifics of the selected custom map cell on-the-fly since they change frequently
	if(selectedCellX != -1 && selectedCellY != -1)
	{
		_itoa_s(selectedCellX,tempBuffer,10);
		applySurface(656,32,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);
		_itoa_s(selectedCellY,tempBuffer,10);
		applySurface(704,32,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);

		// paint information about the background layer if it is initialized
		if(selectedCell->b != -1)
		{
			_itoa_s(selectedCell->b,tempBuffer,10);
			applySurface(720,64,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);
			applySurface(720,80,TTF_RenderText_Solid( fontCalibriTiny, (selectedCell->isWalkable)?"Y":"N", textColor),screen);
		}

		// paint information about the foreground layer if it is initialized
		if(selectedCell->f != -1)
		{
			_itoa_s(selectedCell->f,tempBuffer,10);
			applySurface(720,48,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);
		}

		// paint the selected cell itself
		customMap->paintTile(selectedCellX,selectedCellY,752,16);
	}

	applySurface(640,112,selectedTileMsg,screen);
	applySurface(640,128,indexMsg,screen);

	// paint information about the selected graphics
	if(selectedTileIndex != -1)
	{
		_itoa_s(selectedTileIndex,tempBuffer,10);
		applySurface(688,128,TTF_RenderText_Solid( fontCalibriTiny, tempBuffer, textColor),screen);
		applySurface(752,112,graphics->image,screen,&graphics->clip[selectedTileIndex]);
	}
}

// full layered paint of the map editor and all of its inner sections
void MapEditor::paint()
{
	applySurface(0,0,background,screen);

	if(isLoaded)
	{
		customMap->paint();
		tileSet->paint(selectedTileX,selectedTileY);

		// paint the 4 directional buttons for each scrolling window
		for(int i=0;i<4;i++)
		{
			customMapBtns[i].paint();
			tileSetBtns[i].paint();
		}

		paintInfoPanel();
	}
}

void MapEditor::mouseLeft(int x, int y) 
{
	// check if the custom map was clicked
	if(inBounds(x,y,customMap->limit))
	{
		coordToIndex(&x,&y,customMap);
		selectedCellX = x;
		selectedCellY = y;
		selectedCell = customMap->getTile(selectedCellX,selectedCellY);

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
	else if(inBounds(x,y,tileSet->limit))
	{
		coordToIndex(&x,&y,tileSet);
		selectedTileX = x;
		selectedTileY = y;
		selectedTileIndex = getTileArrayIndex(x,y);
	}
	else if(saveBtn.inBounds(x,y))
	{
		if(currentMapFilename != NULL)
		{
			customMap->save(currentMapFilename);
		}
	}
	else if(closeBtn.inBounds(x,y))
	{
		signalCompletion();
	}
	// check if the scrolling arrows were pressed
	else
	{
		checkButtons(x,y,customMapBtns,customMap);
		checkButtons(x,y,tileSetBtns,tileSet);
	}
}

void MapEditor::mouseRight(int x, int y)
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
	else if(inBounds(x,y,customMap->limit))
	{
		coordToIndex(&x,&y,customMap);
		Tile *tempCell = customMap->getTile(x,y);
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

void MapEditor::showInstructions()
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
