#include "TilePrototype.h"

RESULT HexagonTilePrototype::InitializeData()
{
	float height = HEXAGON_SIZE - HEXAGON_PADDING;
	float width = HEXAGON_SIZE * sqrt(3) / 2 - HEXAGON_PADDING;
	BLOCKALLOC(VertexType[6], pointArray);

	pointArray[0].position = D3DXVECTOR3(0, height / 2, 1);
	pointArray[1].position = D3DXVECTOR3(width / 2, height / 4, 1);
	pointArray[2].position = D3DXVECTOR3(-width / 2, height / 4, 1);
	pointArray[3].position = D3DXVECTOR3(width / 2, -height / 4, 1);
	pointArray[4].position = D3DXVECTOR3(-width / 2, -height / 4, 1);
	pointArray[5].position = D3DXVECTOR3(0, -height / 2, 1);

	pointArray[0].texture = D3DXVECTOR2(0.5, 0);
	pointArray[1].texture = D3DXVECTOR2(1, 0.25);
	pointArray[2].texture = D3DXVECTOR2(0, 0.25);
	pointArray[3].texture = D3DXVECTOR2(1, 0.75);
	pointArray[4].texture = D3DXVECTOR2(0, 0.75);
	pointArray[5].texture = D3DXVECTOR2(0.5, 1);

	pointCount = 6;
	return 0;
}

int HexagonTileDefault::id() { return TILE_DEFAULT_ID; }

const char * HexagonTileDefault::name() { return ""; }

RESULT HexagonTileDefault::TileBehaviour(TileStorage&, GlobalEffect*)
{
	return 0;
}

RESULT HexagonTileDefault::InitializeStorage(TileStorage &)
{
	return 0;
}
