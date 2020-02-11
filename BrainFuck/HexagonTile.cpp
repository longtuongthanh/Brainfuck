#include "HexagonTile.h"

RESULT HexagonTilePrototype::InitializeData()
{
	float height = HEXAGON_SIZE - HEXAGON_PADDING;
	float width = HEXAGON_SIZE * sqrt(3) / 2 - HEXAGON_PADDING;
	BLOCKALLOC(VertexType[6], pointArray);

	D3DXVECTOR3 newVerx[6];
	newVerx[0] = D3DXVECTOR3(0, height / 2, 1);
	newVerx[1] = D3DXVECTOR3(width / 2, height / 4, 1);
	newVerx[2] = D3DXVECTOR3(-width / 2, height / 4, 1);
	newVerx[3] = D3DXVECTOR3(width / 2, -height / 4, 1);
	newVerx[4] = D3DXVECTOR3(-width / 2, -height / 4, 1);
	newVerx[5] = D3DXVECTOR3(0, -height / 2, 1);

	for (int i = 0; i < 6; i++)
	{
		pointArray[i].position = newVerx[i];
	}
	pointCount = 6;
	return 0;
}


TileStorage::TileStorage(const TileStorage & x) {
	for (int i = 0; i < 6; i++) {
		typeID[i] = x.typeID[i];
		amount[i] = x.amount[i];
	}
}

TileStorage::TileStorage() {}
TileStorage::~TileStorage() {}
HexagonTile::HexagonTile() {}
HexagonTile::HexagonTile(Point position) : HexagonTileBase(position) {}

RESULT HexagonTile::Release()
{
	DESTROY(prototype);
	return 0;
}

RESULT HexagonTile::Initialize(ID3D11Device* device, TextureLibrary* textureLib, TextureShader* texShader)
{
	DEBUG(if (prototype) return 1;)
	BLOCKALLOC(HexagonTilePrototype, prototype);
	BLOCKCALL(prototype->Initialize(device, HEXAGON_TEXTURE_FILE, textureLib, texShader),
		"Cannot initiate prototype HexagonTile");
	return 0;
}
