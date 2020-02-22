#pragma once
#include "TextureObject.h"
#include "useful_stuff.h"
#include "HexagonTile.h"
#include "ShaderLibrary.h"
#include "ItemLibrary.h"

#include "TileDetermination.h"
#include "mydeque.h"

class HexagonMap
{
public:
	HexagonMap();
	~HexagonMap();

	HexagonMap& operator=(const HexagonMap &);
	HRESULT Frame(const Point&, GlobalEffect*);

	HRESULT Render(Point, ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	RESULT Initialize(ID3D11Device*, TextureLibrary*, ShaderLibrary*);
	RESULT Release();

	/** Get the coordinate of the hexagon that contains this point.*/
	Coord GetCoord(Point);
	/** Return the center of the hexagon at that coordinate*/
	Point GetLocation(int x, int y);
	mydeque<mydeque<HexagonTile*>>& GetMap();
protected:
	RESULT InitializeData();
	RESULT AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT tileWidth, FLOAT tileHeight);

private:
	int max_X, max_Y, min_X, min_Y; // this show the max and min coord of tile

	std::string textureFile;

	mydeque<mydeque<HexagonTile*>> map;

	TextureLibrary* pTextureLib;
	ShaderLibrary* pShaderLib;
	ID3D11Device* pDevice;
	ItemLibrary* itemLib;
	TileLibrary* tileLib;
};