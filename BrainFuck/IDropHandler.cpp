#include "IDropHandler.h"

IDropHandler::IDropHandler(std::vector<Point>* hitBox, Point* position)
{
	pHitBox = hitBox;
	pPosition = position;
}

bool IDropHandler::Frame(GameObject* gameObject, PointerEventData* pointerEventData)
{
	if (pointerEventData->pointerDrag != nullptr)
	{
		std::vector<Point> transformedHitBox;
		for (auto point : *pHitBox)
		{
			transformedHitBox.push_back(point + *pPosition);
		}

		if (isInside(transformedHitBox, pointerEventData->position))
		{
			OnDrop(pointerEventData);
			return 0;
		}
	}
	return 0;
}

