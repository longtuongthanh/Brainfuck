#pragma once
#include "useful_stuff.h"
#include "PointerEventData.h"

class IPointerDownHandler
{
public:
	IPointerDownHandler(std::vector<Point>* hitBox, Point* position);
	~IPointerDownHandler() {};
	
	RESULT InitData(std::vector<Point>* hitBox, Point* position);

protected:
	virtual void OnPointerDown(PointerEventData*) = 0;

private:
	bool Frame(GameObject*, PointerEventData*);
	friend class EventDistributor;

private:
	std::vector<Point>* pHitBox;
	Point* pPosition;
};