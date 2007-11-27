#ifndef MAPEDITOR_H
	#define MAPEDITOR_H

#include "Graphics.h"
#include "Screen.h"

/* Class EditableMap: extension of the Map class which allows the user to make
 *   changes to the map which has been loaded. Adds the ability to save the map
 *   and remove tile layers (erase).
 */
class EditableMap : public Map
{
private:
	// empty graphic tile to show a tile without assigned graphics
	SDL_Surface *emptyTile;

	void init();

public:
	EditableMap();
	~EditableMap();

	// map is created, passing the specs of its bounding window
	EditableMap(int, int, int, int);

	// creates an empty map with the sizes provided
	void loadEmptyMap(int, int);

	// paints the entire map
	void paint();

	// paints a specific tile, using the empty tile if it hasn't been assigned graphics
	void paintTile(int, int, int = -1, int = -1);

	// saves the currently loaded map to a file
	void save(char*);
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
	SelectableMap();
	~SelectableMap();

	// map is created, passing the specs of its bounding window
	SelectableMap(int, int, int, int);

	// paints all the graphic tiles, including the highlighting of the selected tile
	void paint(int = -1, int = -1);

	void loadGraphics(Graphics*);
};


/* Class MapEditor: Uses the Map class and its child classes to make a map editor.
 *   Allows maps to be loaded, edited and saved.
 */
class MapEditor :  public Screen
{
private:
	SDL_Surface *background, *arrows;

	// tracks the indexes of selected map cells and graphic tiles
	int selectedTileX, selectedTileY, selectedTileIndex, selectedCellX, selectedCellY;
	Tile *selectedCell;

	// rendered text to inform the user
	SDL_Surface *selectedTileMsg, *selectedCellMsg, *xCoordLabel, *yCoordLabel, *foregroundMsg, *backgroundMsg, *indexMsg, *walkableMsg;

	// pointer to the graphics which will be used to build a map
	Graphics *graphics;

	// the editable map which will be where you create your new map
	EditableMap *customMap;
	Button customMapBtns[4];

	// a selectable map, which will store the full set of tiles you can use to create a map
	SelectableMap *tileSet;
	Button tileSetBtns[4];

	// buttons in a fixed location
	Rect closeBtn, saveBtn;

	// the tile size corresponding to the tiles used to create the map
	int tileSize;

	// has a map been loaded successfully?
	bool isLoaded;

	// the filename of the current map, if it was loaded from disk, this will allow saving on top of it
	char* currentMapFilename;

	// checks if an x,y coordinate is within the bounds of a certain screen area
	bool inBounds(int, int, Rect);

	// converts an x,y coordinate in pixels into a 2-d array index for the corresponding tile
	void coordToIndex(int*, int*, Map*);

	// converts an x,y coordinate (in tiles) into a 1-d array index
	int getTileArrayIndex(int, int);

	// checks if the scroll buttons were pressed, if so, it invokes the appropriate scrolling
	void checkButtons(int, int, Button*, Map*);

	// shows the informative panel
	void paintInfoPanel();

	// release any memory used for the editing of a specific map
	void unload();

	static const int DEFAULT_NEW_MAP_SIZE_W = 20;
	static const int DEFAULT_NEW_MAP_SIZE_H = 20;
	static const char* DEFAULT_TILES;
	static const char* DEFAULT_MAP_LOCATION;
	static const int DEFAULT_TILES_R = 0xFF;
	static const int DEFAULT_TILES_G = 0xE2;
	static const int DEFAULT_TILES_B = 0xAA;
public:
	MapEditor(int,int,int,int);
	~MapEditor();
	void mouseLeft(int, int);
	void mouseRight(int, int);
	void showInstructions();

	// loads a map from disk, or initializes a new empty map
	void load(const char* = NULL, const char* = NULL, int = DEFAULT_NEW_MAP_SIZE_W, int = DEFAULT_NEW_MAP_SIZE_H);

	// full layered paint of the map editor and all of its inner sections
	void paint();

	// if a custom map cell is selected, switch between walkable and non-walkable
	void toggleWalkable();
};

#endif