#pragma once

#include "TilePrototype.h"

#define TILE_DETERMINATION_ID 1

static const CHAR* DETERMINATION_TEXTURE_FILE = "texture.dds";
class TileDetermination : public HexagonTilePrototype {
public:
	int id();
	const char* name();
	RESULT Frame();
	RESULT TileBehaviour(TileStorage&, GlobalEffect*, Coord&);
	RESULT InitializeStorage(TileStorage&);
	RESULT AddItem(TileStorage&, int);
};