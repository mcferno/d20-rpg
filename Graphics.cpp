#include "Graphics.h"

// #####################################################################################################

SDL_Surface *loadImage( const char *filename, int r, int g, int b ) 
{ 
	SDL_Surface* loadedImage = NULL; 
	SDL_Surface* optimizedImage = NULL;

	// load the image from disk
	std::string image_path = getResourcePath(filename);
	loadedImage = IMG_Load(image_path.c_str());

	if( loadedImage == NULL ) 
	{ 
		std::cout << "loadImage error: " << filename << " could not be loaded\n";
		return NULL;
	}

	// creates optimized version of the image, matching the screen's bit-depth
	optimizedImage = SDL_DisplayFormat( loadedImage ); 
	SDL_FreeSurface( loadedImage );

	if(optimizedImage == NULL)
	{ 
		std::cout << "loadImage error: " << filename << " could not be optimized\n";
		return NULL;
	}

	// if a color key has been provided, mask that color out from the image.
	if(r != -1 || g != -1 || b != -1 ) 
	{ 
		Uint32 colorkey = SDL_MapRGB( optimizedImage->format, r, g, b );
		
		//Set all pixels matching the color key to transparent
		SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey ); 
	}

	return optimizedImage; 
}
/*
SDL_Surface *loadImage(const char *filename, int r, int g, int b ) 
{ 
	return (loadImage(const_cast<char*>(filename),r,g,b));
}
*/

void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip ) 
{ 
	SDL_Rect offset; 
	
	//Give the offsets to the rectangle because SDL_BlitSurface() only accepts the offsets inside of an SDL_Rect. 
	offset.x = x; 
	offset.y = y;
	
	//Blit the surface 
	SDL_BlitSurface( source, clip, destination, &offset ); 
}

// #####################################################################################################


Tile::Tile()
{
		isWalkable = true;
		f = b = GRAPHICS_INT_DEFAULT;
}

Tile::Tile(int newB, int newF = GRAPHICS_INT_DEFAULT, bool newWalkable = true)
{
		b = newB;
		f = newF;
		isWalkable = newWalkable;
}

// #####################################################################################################

Rect::Rect(int newX, int newY, int newW, int newH) 
{
	x = newX;
	y = newY;
	w = newW;
	h = newH;
}

bool Rect::inBounds(int clickX, int clickY)
{
	return (clickX >= x && clickX <= x+w && clickY >= y && clickY <= y+h);
}

// #####################################################################################################

Button::Button(int newX, int newY, int newW, int newH, int newClipX, int newClipY, SDL_Surface* newImage) :
	Rect(newX,newY,newW,newH)
{
	clip.x = newClipX;
	clip.y = newClipY;
	clip.w = w;
	clip.h = h;
	image = newImage;
}

void Button::paint(int paddingX, int paddingY)
{
	if(image != NULL)
		applySurface(paddingX+x,paddingY+y,image,screen,&clip);
}

// #####################################################################################################

void Graphics::chopUp()
{
	int xOffset = 0;
	int yOffset = 0;

	//chop the tileset into the appropriate cells
	for(int i=0;i<totalTiles;i++)
	{
		clip[i].x = xOffset;
		clip[i].y = yOffset;
		clip[i].w = tileSize;
		clip[i].h = tileSize;

		// make sure to not read beyond the width of the image.
		// load the image row by row
		if((i+1) % wTiles == 0)
		{
			yOffset += tileSize;
			xOffset = 0;
		} else
			xOffset += tileSize;
		
	}
}

Graphics::Graphics()
{}

Graphics::Graphics(const char *filename, int r, int g, int b, int newTileSize)
{
	std::cout << "Opening graphics file \"" << filename << "\" ... ";
	image = loadImage(filename,r,g,b);

	if(image == NULL)
	{
		std::cout << "FAILED!\n";
		return;
	}
	std::cout << "Successful!\n";

	tileSize = newTileSize;
	init();
}

Graphics::Graphics(char *filename, int r, int g, int b, int newTileSize)
{
	std::cout << "Opening graphics file \"" << filename << "\" ... ";
	image = loadImage(filename,r,g,b);

	if(image == NULL)
	{
		std::cout << "FAILED!\n";
		return;
	}
	std::cout << "Successful!\n";

	tileSize = newTileSize;
	init();
}

void Graphics::init()
{
	x = y = 0;
	w = image->w;
	h = image->h;
	wTiles = w/tileSize;  // check if its an even division
	hTiles = h/tileSize;
	totalTiles = wTiles*hTiles;

	clip = new SDL_Rect[totalTiles];

	// populate the 2d array
	chopUp();
}

Graphics::~Graphics()
{
	SDL_FreeSurface(image);
	delete[] clip;
}


// #####################################################################################################

Map::Map()
{
	ts = NULL;
}

Map::Map(int newX, int newY, int newW, int newH)
{
	x = 0;
	y = 0;
	limit.x = newX;
	limit.y = newY;
	limit.w = newW;
	limit.h = newH;

	ts = NULL;
}

Map::~Map()
{
	if(ts != NULL)
	{
		for (int k = 0; k < w; k++)
			delete[] ts[k];
		delete[] ts;
	}
}

void Map::parseIndex(char *filename)
{
	int mapW, mapH, newF, newB;
	char newW;

	std::ifstream mapFile(filename);

	std::cout << "Opening map index \"" << filename << "\" ... ";

	// verify that the file was opened correctly before reading 
	if(mapFile.bad()) 
	{ 
		std::cout << "FAILED!\n";
		return; 
	}
	std::cout << "Successful!\n";

	// get the width and height of the map (according to its index)
	mapFile >> mapW;
	mapFile >> mapH;

	w = mapW;
	h = mapH;

	// check if the bounding window is larger than the map
	if(limitWTiles > w)
		limitWTiles = w;
	if(limitHTiles > h)
		limitHTiles = h;


	// initialize 2D array of tiles which will represent the map
	ts = new Tile*[mapW];
	for (int k = 0; k < mapW; k++)
		ts[k] = new Tile[mapH];


	std::cout << "Initializing map, W: "<<mapW<<" tiles, H: "<< mapH <<" tiles ... ";

	// read the input index file, initializing the tiles
	for(int i=0;i<w;i++)
	{
		for(int j=0;j<h;j++)
		{
			mapFile >> newB;
			mapFile >> newF;
			mapFile >> newW;

			ts[i][j].b = newB;
			ts[i][j].f = newF;
			ts[i][j].isWalkable = (newW=='Y')?true:false;
		}
	}
	mapFile.close();
	std::cout << "Successful!\n";
}

void Map::parseIndex(const char *filename)
{
	parseIndex(const_cast<char*>(filename));
}

void Map::loadGraphics(char *filename, int alphaR, int alphaG, int alphaB)
{
	loadGraphics(new Graphics(filename,alphaR,alphaG,alphaB));
}

void Map::loadGraphics(const char *filename, int alphaR, int alphaG, int alphaB)
{
	loadGraphics(const_cast<char*>(filename),alphaR, alphaG, alphaB);
}

void Map::loadGraphics(Graphics *newGraphics)
{
	g = newGraphics;
	tileSize = g->tileSize;
	limitWTiles = limit.w / tileSize;
	limitHTiles = limit.h / tileSize;
}
void Map::loadMap(Graphics *newGraphics, char *indexFilename)
{
	loadGraphics(newGraphics);
	parseIndex(indexFilename);
}

void Map::loadMap(Graphics *newGraphics, const char *indexFilename)
{
	loadMap(newGraphics,const_cast<char*>(indexFilename));
}

void Map::scrollLeft()
{
	if(x > 0)
		x--;
}
void Map::scrollRight()
{
	// check if there are more tiles to scroll to
	if(w - x > limitWTiles)
		x++;
}
void Map::scrollUp()
{
	if(y > 0)
		y--;
}
void Map::scrollDown()
{
	// check if there are more tiles to scroll to
	if(h - y > limitHTiles)
		y++;
}

bool Map::isOnScreen(int newX, int newY)
{
	return (newX >= x && newX < x + limitWTiles && newY >= y && newY < y + limitHTiles);
}

void Map::paint()
{
	//std::cout << "limitWTiles: " << limitWTiles << ", limitHTiles:" << limitHTiles << "\n";
	for(int i=x;i<x+limitWTiles;i++)
		for(int j=y;j<y+limitHTiles;j++)
			paintTile(i,j);
}

// paints a specific tile, using the empty tile if it hasn't been assigned graphics
void Map::paintTile(int indexX, int indexY, int locX, int locY)
{
	// paint elsewhere than the default location?
	if(locX == -1)
		locX = limit.x+(indexX-x)*tileSize;
	if(locY == -1)
		locY = limit.y+(indexY-y)*tileSize;

	applySurface(locX,locY,g->image,screen,&g->clip[ts[indexX][indexY].b]);
	if(ts[indexX][indexY].f != -1)
		applySurface(locX,locY,g->image,screen,&g->clip[ts[indexX][indexY].f]);
}

Tile* Map::getTile(int indexX, int indexY)
{
	// verify that the provided index is within the bounds of the map
	if(indexX < 0 || indexX >= w || indexY < 0 || indexY >= h)
		return NULL;
	return &ts[indexX][indexY];
}
