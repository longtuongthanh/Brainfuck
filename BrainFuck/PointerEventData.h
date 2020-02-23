#pragma once
#include "useful_stuff.h"
#include "GameObject.h"

class PointerEventData
{
public:
	Point position; // Current position of mouse to world
	Point delta; // Pointer delta since last update
	int mouseFlag; // Current mouse flag
	GameObject* pointerDrag; // Pointer to gameObject that receive OnDrag
	GameObject* pointerPress; // Pointer to gameObject that receive OnPointerDown
};