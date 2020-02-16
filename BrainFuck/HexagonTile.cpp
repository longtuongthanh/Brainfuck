#include "HexagonTile.h"

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

HexagonTile::HexagonTile() {}
HexagonTile::HexagonTile(Point position) : HexagonTileMiddle(position) {}

RESULT HexagonTile::Release()
{
	DESTROY(prototype);
	return 0;
}

RESULT HexagonTile::Initialize(ID3D11Device* device, TextureLibrary* textureLib, TextureShader* texShader, ItemLibrary* itemLib)
{
	__super::Initialize(device, textureLib, texShader, itemLib);
	DEBUG(if (prototype) return 1;)
	BLOCKALLOC(HexagonTilePrototype, prototype);
	BLOCKCALL(prototype->Initialize(device, HEXAGON_TEXTURE_FILE, textureLib, texShader),
		"Cannot initiate prototype HexagonTile");
	return 0;
}
