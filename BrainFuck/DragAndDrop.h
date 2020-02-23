#pragma once
#include "useful_stuff.h"

class DragAndDrop
{
public:
    void UpdateDragAndDrop(Point& pos, std::vector<Point> hitBox, class Input& input, Point camPos);

private:

private:
    bool beingHold = false;
    bool clickable = false;
    Point prevPos;

};