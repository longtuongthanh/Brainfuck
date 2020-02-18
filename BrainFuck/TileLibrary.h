#pragma once

#include "TilePrototype.h"
#include "TileDetermination.h"

class TileLibrary : NonCopyable
{
	HexagonTilePrototype* prototype[NO_OF_DIF_ITEM];
public:
	TileLibrary() : prototype() {};
	~TileLibrary() {};

	// TODO: ADD ANY TILE OBJECT HERE
	RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*);
	RESULT Release();

	RESULT AddTile(HexagonTilePrototype*);

	HexagonTilePrototype* GetPrototype(int id);
};