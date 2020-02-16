#include "DragAndDrop.h"
#include "Input.h"



void DragAndDrop::UpdateDragAndDrop(Point& pos, std::vector<Point> hitBox, Input& input, Point camPos)
{
    bool mouseInside = isInside(hitBox, input.MouseToField() + camPos);

    if (beingHold)
    {
        if (input.MouseFlag() & 0x0001)
        {
            pos += input.MouseToField() + camPos - prevPos;
        }
        else
        {
            beingHold = false;
        }
    }
    else if (input.MouseFlag() & 0x0001 & clickable)
    {
        beingHold = true;
    }
    else if (!(input.MouseFlag() & 0x0001) & mouseInside)
    {
        clickable = true;
    }
    else
    {
        clickable = false;
    }
    prevPos = input.MouseToField() + camPos;
}
