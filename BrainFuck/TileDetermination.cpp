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
	storage.InitializeData(7, 0, 0, 0, 0, 0, 0, 0);
	return 0;
}

RESULT TileDetermination::TileBehaviour(TileStorage&, GlobalEffect*) {
	return 0;
}
