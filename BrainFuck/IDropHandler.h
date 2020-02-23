#pragma once
#include "useful_stuff.h"
#include "PointerEventData.h"
#include "GameObject.h"

class IDropHandler
{
public:
	IDropHandler(std::vector<Point>* hitBox, Point* position);
	~IDropHandler() {};

protected:
	virtual void OnDrop(PointerEventData*) = 0;

private:
	bool Frame(GameObject*, PointerEventData*);
	friend class EventDistributor;

private:
	std::vector<Point>* pHitBox;
	Point* pPosition;
};