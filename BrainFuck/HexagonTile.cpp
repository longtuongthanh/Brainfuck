#include "HexagonTile.h"

HexagonTile::Storage::Storage()
{
}

HexagonTile::Storage::Storage(const Storage&)
{
}

HexagonTile::Storage::~Storage()
{
}

HexagonTile::HexagonTile()
{
	position = Point(0, 0);
}

HexagonTile::HexagonTile(Point position, FLOAT width, FLOAT height)
{
	this->position = position;
	this->width = width;
	this->height = height;
}

HexagonTile::HexagonTile(const HexagonTile&)
{
}

HexagonTile::~HexagonTile()
{
}

Point HexagonTile::GetPosition()
{
	return position;
}

RESULT HexagonTile::InitializeData()
{
	BLOCKALLOC(VertexType[6], pointArray);

	D3DXVECTOR3 newVerx[6];
	newVerx[0] = D3DXVECTOR3(position.x + 0, position.y + height / 2, 1);
	newVerx[1] = D3DXVECTOR3(position.x + width / 2, position.y + height / 4, 1);
	newVerx[2] = D3DXVECTOR3(position.x - width / 2, position.y + height / 4, 1);
	newVerx[3] = D3DXVECTOR3(position.x + width / 2, position.y - height / 4, 1);
	newVerx[4] = D3DXVECTOR3(position.x - width / 2, position.y - height / 4, 1);
	newVerx[5] = D3DXVECTOR3(position.x + 0, position.y - height / 2, 1);

	for (int i = 0; i < 6; i++)
	{
		pointArray[i].position = newVerx[i];
	}
	pointCount = 6;
	return RESULT();
}
