#include "HexagonTile.h"

HexagonTile::HexagonTile() { id = 0; }

HexagonTile::HexagonTile(Point position, int id) : position(position) , id(id) {}


HexagonTile::HexagonTile(const HexagonTile & x) : position(x.position), storage(x.storage) {}

Point HexagonTile::GetPosition() { return position; }

RESULT HexagonTile::Frame(GlobalEffect* effect) { 
	return tileLib->GetPrototype(id)->TileBehaviour(storage, effect); 
}

RESULT HexagonTile::Release()
{
	delete this;
	return 0;
}
RESULT HexagonTile::Initialize(ItemLibrary * itemLib, TileLibrary* tileLib) {
	BLOCKCALL(storage.Initialize(itemLib), "cannot initialize tile storage");
	tileLib->GetPrototype(id)->InitializeStorage(storage);

	this->tileLib = tileLib;
	return 0;
}

RESULT HexagonTile::FramePrototype() { return tileLib->GetPrototype(id)->Frame(); }

RESULT HexagonTile::Render(ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	WorldMatrix(worldMatrix);
	BLOCKCALL(tileLib->GetPrototype(id)->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix),
		"cannot render prototype of tile");
	BLOCKCALL(storage.Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix),
		"cannot render storage.");
	return 0;
}

void HexagonTile::WorldMatrix(D3DXMATRIX & worldMatrixOriginal)
{
	D3DXMATRIX newMatrix;
	D3DXMatrixTranslation(&newMatrix, position.x, position.y, 0);
	worldMatrixOriginal *= newMatrix;
}

RESULT HexagonTile::ChangeType(int id)
{
	this->id = id;
	tileLib->GetPrototype(id)->InitializeStorage(storage);
	return 0;
}
