#pragma once

#include "useful_stuff.h"
#include "TextureObject.h"
#include "TileStorage.h"
#include "GlobalEffect.h"

//singleton for rendering
// Its called Flyweight, not prototype... but I like the name prototype better.
class HexagonTilePrototype : public TextureObject
{
public:
	HexagonTilePrototype() {}
	~HexagonTilePrototype() {}
	virtual int id() = 0;
	virtual const char* name() = 0;

	/** Indicate whether a tile should change into another type*/
	virtual int TileChange() { return -1; }
	
	/** Tile behaviour. indicate what the tile do*/
	virtual RESULT TileBehaviour(TileStorage&, GlobalEffect*, Coord&) = 0;
	
	/** How storage should be initialized*/
	virtual RESULT InitializeStorage(TileStorage&) = 0;
	
	/** How item should be added to tile*/
	virtual RESULT AddItem(TileStorage&, int);
protected:
	RESULT InitializeData(); // We can use this to initialize data storage or behavior
};

// default tile
#define TILE_DEFAULT_ID 0
static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";
class HexagonTileDefault final : public HexagonTilePrototype {
public:
	HexagonTileDefault() {}
	~HexagonTileDefault() {}
	int id();
	const char* name();
	RESULT TileBehaviour(TileStorage&, GlobalEffect*, Coord&);
	RESULT InitializeStorage(TileStorage&);
};
/*
#define TILE_BASIC_STORAGE_ID 2
static const CHAR* TILE_BASIC_STORAGE_FILE = "texture.dds";
class HexagonTileBasicStorage final : public HexagonTilePrototype {
public:
	HexagonTileBasicStorage() {}
	~HexagonTileBasicStorage() {}
	int id();
	const char* name();
	RESULT TileBehaviour(TileStorage&, GlobalEffect*);
	RESULT InitializeStorage(TileStorage&);
};
//*/