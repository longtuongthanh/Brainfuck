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
	virtual int TileChange() { return -1; }
	virtual RESULT TileBehaviour(TileStorage&, GlobalEffect*) = 0;
	virtual RESULT InitializeStorage(TileStorage&) = 0;
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
	RESULT TileBehaviour(TileStorage&, GlobalEffect*);
	RESULT InitializeStorage(TileStorage&);
};