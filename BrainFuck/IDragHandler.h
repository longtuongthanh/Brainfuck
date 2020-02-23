#pragma once
#include "useful_stuff.h"
#include "PointerEventData.h"

class IDragHandler
{
public:
	IDragHandler(std::vector<Point>* hitBox, Point* position);
	~IDragHandler() {};

protected:
	virtual void OnDrag(PointerEventData*) = 0;

private:
	void Prepare(GameObject*, PointerEventData*);
	bool Frame(GameObject*, PointerEventData*);
	friend class EventDistributor;

private:
	std::vector<Point>* pHitBox;
	Point* pPosition;
	bool beingClicked = false;
	Point prevMousePosition;
};