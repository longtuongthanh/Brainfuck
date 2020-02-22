#include "TileDetermination.h"

RESULT TileDetermination::Frame() {
	return 0;
}

int TileDetermination::id() {
	return TILE_DETERMINATION_ID;
}

const char* TileDetermination::name() {
	return "Determination";
}

RESULT TileDetermination::InitializeStorage(TileStorage& storage) {
	storage.InitializeData(1, ITEM_WILL_ID);
	return 0;
}

RESULT TileDetermination::AddItem(TileStorage& storage, int itemID)
{
	if (itemID != 0 || storage.amount[0] >= 3 || storage.AddItem(itemID))
		return 1;
	return 0;
}

RESULT TileDetermination::TileBehaviour(TileStorage& storage, GlobalEffect* globalEffect, Coord& coord) {
	DEBUG(if (storage.typeID[0] != 0) {
		cerr << "wrong typeID";
		return 1;
	});
	if (!globalEffect->IsNewTurn())
		return 0;
	if (storage.amount[0] >= 3)
		globalEffect->OverflowReport(coord.x, coord.y, 0);
	else
		storage.AddItem(0);
	return 0;
}
