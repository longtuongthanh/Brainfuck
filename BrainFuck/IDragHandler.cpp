#include "IDragHandler.h"

IDragHandler::IDragHandler(std::vector<Point>* hitBox, Point* position)
{
	this->pHitBox = hitBox;
	this->pPosition = position;
}

void IDragHandler::Prepare(GameObject* gameObject, PointerEventData* pointerEventData)
{
	std::vector<Point> transformedHitBox;
	for (auto point : *pHitBox)
	{
		transformedHitBox.push_back(point + *pPosition);
	}

	if (isInside(transformedHitBox, pointerEventData->position))
	{
		beingClicked = true;
		prevMousePosition = pointerEventData->position;
	}
}

bool IDragHandler::Frame(GameObject* gameObject, PointerEventData* pointerEventData)
{
	if (pointerEventData->mouseFlag & 0x0001 && 
		beingClicked && 
		prevMousePosition != pointerEventData->position)
	{
		prevMousePosition = pointerEventData->position;
		pointerEventData->pointerDrag = gameObject;
		OnDrag(pointerEventData);
		return 0;
	}

	if (!(pointerEventData->mouseFlag & 0x0001))
	{
		beingClicked = false;
		return 1;
	}

	return 0;
}
