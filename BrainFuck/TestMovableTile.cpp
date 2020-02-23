#include "TestMovableTile.h"

TestMovableTile::TestMovableTile(Point position, int id, EventDistributor* eventDistributor)
	:
	HexagonTile(position,id),
	IPointerDownHandler(&hitBox, &this->position), // set up hitbox and position for IPointerDownHandler
	IDragHandler(&hitBox, &this->position),
	IDropHandler(&hitBox, &this->position)
{
	eventDistributor->SubscribePointerDown(this, this);
	eventDistributor->SubscribeOnDrag(this, this);
	eventDistributor->SubscribeOnDrop(this, this);
}

void TestMovableTile::OnPointerDown(PointerEventData* pointerEventData)
{
	std::cout << std::endl << "Clicked";
	beginDragPosition = position;
}

void TestMovableTile::OnDrag(PointerEventData* pointerEventData)
{
	std::cout << std::endl << "On Drag";
	this->position = pointerEventData->position;
	canDoDropFunc = true;
}

void TestMovableTile::OnDrop(PointerEventData*)
{
	std::cout << std::endl << "On Drop";
	if (canDoDropFunc)
	{
		position = beginDragPosition;
	}
}

RESULT TestMovableTile::Initialize(ItemLibrary* itemLib, TileLibrary* tileLib)
{
	HexagonTile::Initialize(itemLib, tileLib);
	InitializeHitBox();
	return 0;
}

RESULT TestMovableTile::SetPosition(Point newPosition)
{
	this->position = newPosition;
	return 0;
}

RESULT TestMovableTile::InitializeHitBox()
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
