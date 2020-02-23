#pragma once
#include "HexagonTile.h"
#include "IPointerDownHandler.h"
#include "EventDistributor.h"
#include "IDragHandler.h"
#include "IDragHandler.h"

class TestDropTile : public HexagonTile, public IDropHandler
{
public:
	TestDropTile(Point position, int id, EventDistributor*);
	void OnDrop(PointerEventData*);

	RESULT Initialize(ItemLibrary*, TileLibrary*);
	RESULT InitializeHitBox();

private:
	std::vector<Point> hitBox;

private:
	Point beginDragPosition;
};