#include "TileLibrary.h"

RESULT TileLibrary::Initialize(ID3D11Device* device, TextureLibrary* textureLib, TextureShader* texShader)
{
	HexagonTilePrototype* tile;
	BLOCKALLOC(HexagonTileDefault, tile);
	BLOCKCALL(tile->Initialize(device, HEXAGON_TEXTURE_FILE, textureLib, texShader),
		"Cannot initiate prototype HexagonTileDefault");
	AddTile(tile);

	BLOCKALLOC(TileDetermination, tile);
	BLOCKCALL(tile->Initialize(device, DETERMINATION_TEXTURE_FILE, textureLib, texShader),
		"Cannot initiate prototype HexagonTileDefault");
	AddTile(tile);

	return 0;
}
RESULT TileLibrary::Release()
{
	for (auto tile : prototype)
		DESTROY(tile);
	delete this;
	return 0;
}

RESULT TileLibrary::AddTile(HexagonTilePrototype * tile)
{
	int id = tile->id();
	DEBUG(
		if (prototype[id]) {
			cerr << "WARNING: ID COLLISION";
			return 1;
		}
	)
		prototype[id] = tile;
	return 0;
}

HexagonTilePrototype* TileLibrary::GetPrototype(int id)
{
	DEBUG(
		if (prototype[id] == NULL) {
			cerr << "ERROR: ITEM NOT AVAILABLE.";
		}
	)
		return prototype[id];
}
