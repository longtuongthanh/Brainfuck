#include "IPointerDownHandler.h"
#include "TestMovableTile.h"

IPointerDownHandler::IPointerDownHandler(std::vector<Point>* hitBox, Point* position)
{
	this->pHitBox = hitBox;
	this->pPosition = position;
}

RESULT IPointerDownHandler::InitData(std::vector<Point>* hitBox, Point* position)
{
	this->pHitBox = hitBox;
	this->pPosition = position;
	return 0;
}

bool IPointerDownHandler::Frame(GameObject* gameObject, PointerEventData* pointerEventData)
{
	std::vector<Point> transformedHitBox;
	for (auto point : *pHitBox)
	{
		transformedHitBox.push_back(point + *pPosition);
	}

	if (isInside(transformedHitBox, pointerEventData->position))
	{
		pointerEventData->pointerPress = gameObject;
		OnPointerDown(pointerEventData);
		return 0;
	}

	return 1;
}
