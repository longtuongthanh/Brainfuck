#pragma once
#include "HexagonTile.h"
#include "IPointerDownHandler.h"
#include "EventDistributor.h"
#include "IDragHandler.h"

class TestMovableTile: public HexagonTile, public IPointerDownHandler, public IDragHandler, public IDropHandler
{
public:
	TestMovableTile(Point position, int id, EventDistributor*);
	void OnPointerDown(PointerEventData*);
	void OnDrag(PointerEventData*);
	void OnDrop(PointerEventData*);

	RESULT Initialize(ItemLibrary*, TileLibrary*);
	RESULT SetPosition(Point newPosition);

private:
	RESULT InitializeHitBox();

private:
	std::vector<Point> hitBox;

private:
	Point beginDragPosition;

public:
	bool canDoDropFunc;
};