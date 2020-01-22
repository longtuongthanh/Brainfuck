#pragma once
#include "TextureObject.h"

static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";

class HexagonMap: public TextureObject
{
private:
	FLOAT tileWidth, tileHeight; // width and height of hexagon
	FLOAT padding; // space between 2 hexagons

public:
	HexagonMap();
	HexagonMap(FLOAT tileWidth, FLOAT tileHeight, FLOAT padding);
	~HexagonMap();

	HexagonMap& operator=(const HexagonMap &);

protected:
	RESULT InitializeData();
	RESULT AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT tileWidth, FLOAT tileHeight);
};