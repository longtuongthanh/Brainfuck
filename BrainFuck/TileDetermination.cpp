#include "TileDetermination.h"

TileDetermination::TileDetermination()
{
	
}

TileDetermination::TileDetermination(Point position): HexagonTileMiddle(position)
{
	storage.InitializeData(7,0,0,0,0,0,0,0);
}

RESULT TileDetermination::Release()
{
	DESTROY(prototype);
	return 0;
}

RESULT TileDetermination::Initialize(ID3D11Device* device, TextureLibrary* textureLib, TextureShader* texShader, ItemLibrary* itemLib)
{
	__super::Initialize(device, textureLib, texShader, itemLib);
	DEBUG(if (prototype) return 1);
	BLOCKALLOC(HexagonTilePrototype, prototype);
	BLOCKCALL(prototype->Initialize(device, HEXAGON_TEXTURE_FILE, textureLib, texShader),
		"Cannot initiate prototype TileDetermination");

	return 0;
}
RESULT TileDetermination::Frame()
{
	storage.amount[0] = 1;
	return 0;
}