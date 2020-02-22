#include "TileStorage.h"

TileStorage::TileStorage(const TileStorage & x) {
	for (int i = 0; i < 6; i++) {
		typeID[i] = x.typeID[i];
		amount[i] = x.amount[i];
	}
}

TileStorage::TileStorage() {
	ntypes = 0;
}
RESULT TileStorage::InitializeData(unsigned char types, int typeIDs ...)
{
	va_list args;
	va_start(args, types);

	this->ntypes = types;

	for (int i = 0; i < types; i++) {
		int temp = va_arg(args, int);
		if (temp > 0) {
			typeID[i] = temp;
			any.set(i, false);
		}
		else {
			any.set(i, true);
		}
		amount[i] = 0;
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
	for (int i = 0; i < ntypes; i++)
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
	for (int i = 0; i < ntypes; i++)
		if (amount[i] > 0)
			BLOCKCALL(itemLib->GetPrototype(typeID[i])->Render(deviceContext, offsetType(ntypes, i), worldMatrix, viewMatrix, projectionMatrix, amount[i]),
				"cannot render item");
	return 0;
}

int TileStorage::offsetType(int types, int current)
{
	return OFFSET_TYPE[types - 1][current];
}

RESULT TileStorage::AddItem(unsigned char itemID)
{
	for (int i = 0; i < ntypes; i++)
		if (typeID[i] == itemID) {
			amount[i]++;
			return 0;
		}
	for (int i = 0; i < ntypes; i++)
		if (any[i] && amount[i] == 0) {
			typeID[i] = itemID;
			amount[i]++;
			return 0;
		}
	return 1;
}
