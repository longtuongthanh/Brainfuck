#include "TestDropTile.h"
#include "TestMovableTile.h"

TestDropTile::TestDropTile(Point position, int id, EventDistributor* eventDistributor)
	:
	HexagonTile(position, id),
	IDropHandler(&hitBox, &this->position)
{
	eventDistributor->SubscribeOnDrop(this, this);
}

void TestDropTile::OnDrop(PointerEventData* pointerEventData)
{
	TestMovableTile* objectBeingDrag = static_cast<TestMovableTile*>(pointerEventData->pointerDrag);
	if (objectBeingDrag == nullptr)
	{
		return;
	}
	objectBeingDrag->SetPosition(this->position);
	objectBeingDrag->canDoDropFunc = false;
}

RESULT TestDropTile::Initialize(ItemLibrary* itemLib, TileLibrary* tileLib)
{
	HexagonTile::Initialize(itemLib, tileLib);
	InitializeHitBox();
	return 0;
}

RESULT TestDropTile::InitializeHitBox()
{
	float height = HEXAGON_SIZE - HEXAGON_PADDING;
	float width = HEXAGON_SIZE * sqrt(3) / 2 - HEXAGON_PADDING;

	hitBox.push_back(Point(0, height / 2));
	hitBox.push_back(Point(-width / 2, height / 4));
	hitBox.push_back(Point(-width / 2, -height / 4));
	hitBox.push_back(Point(0, -height / 2));
	hitBox.push_back(Point(width / 2, -height / 4));
	hitBox.push_back(Point(width / 2, height / 4));

	return 0;
}
