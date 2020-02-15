#include "TileStorage.h"

TileStorage::TileStorage(const TileStorage & x) {
	for (int i = 0; i < 6; i++) {
		typeID[i] = x.typeID[i];
		amount[i] = x.amount[i];
	}
}

TileStorage::TileStorage() {
	types = 0;
}
RESULT TileStorage::InitializeData(char types, char typeIDs ...)
{
	va_list args;
	va_start(args, types);

	this->types = types;

	for (int i = 0; i < types; i++) {
		typeID[i] = va_arg(args, char);
		DEBUG(if (typeID[i] > NO_OF_DIF_ITEM) return 1;)
	}
		
	va_end(args);
	return 0;
}
TileStorage::~TileStorage() {}

RESULT TileStorage::Initialize(ItemLibrary* itemLib)
{
	this->itemLib = itemLib;
	return 0;
}

RESULT TileStorage::Frame()
{
	for (int i = 0; i < types; i++)
		for (int j = 0; j < amount[i]; j++)
			switch (itemLib->GetPrototype(i)->FrameBehaviour())
			{
				//TODO: CHECK FRAME CODE
			case CODE_KILL:
				amount[i]--;
				break;
			case CODE_ERROR:
				cerr << "Item error";
				break;
			case 0:
				break;
			default:
				cerr << "Unknown item frame code";
				break;
			}
	return 0;
}

RESULT TileStorage::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
							D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	for (int i = 0; i < types; i++)
		BLOCKCALL(itemLib->GetPrototype(typeID[i])->Render(deviceContext, offsetType(types, i), worldMatrix, viewMatrix, projectionMatrix),
			"cannot render item");
	return 0;
}

int TileStorage::offsetType(int types, int current)
{
	return OFFSET_TYPE[types - 1][current];
}
