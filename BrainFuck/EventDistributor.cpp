#include "EventDistributor.h"



EventDistributor::EventDistributor()
{
	pInput = 0;
}


EventDistributor::~EventDistributor()
{
}

RESULT EventDistributor::Initialize(Input* input)
{
	this->pInput = input;
	return 0;
}

RESULT EventDistributor::Frame(CameraClass* pCamera)
{
	Point curPointerPosition = pInput->MouseToField() + Point(pCamera->position.x, pCamera->position.y);

	pointerEventData.delta =  curPointerPosition - pointerEventData.position;
	pointerEventData.mouseFlag = pInput->MouseFlag();
	pointerEventData.position = curPointerPosition;

	char i = 0;
	for (int j = 0; j < 255; j++) {
		i = j;
		switch (pInput->keyboard(i))
		{
		case KEY_STATE_ON_DOWN:
			for (auto command : onKeyDown[i])
				command->Invoke(&i);
			// no break;
		case KEY_STATE_DOWN:
			for (auto command : whileKeyDown[i])
				command->Invoke(&i);
			break;
		default:
			break;
		}
	}
	if (pInput->MouseFlag() & 0x0100) {
		for (auto item : onMouseClick)
		{
			void* result = item.second(pInput->MouseToScreen());
			if (result != NULL)
				item.first->Invoke(result);
		}
	}

	// OnPointerDown
	int nObjectGetPointerDown = 0;
	if (pInput->MouseFlag() & 0x0001 && prevMouseFlag != 0x0001)
	{
		for (auto i : pointerDown)
		{
			if (i.second->Frame(i.first, &pointerEventData) == 0)
			{
				nObjectGetPointerDown++;
			}
		}
	}
	if (nObjectGetPointerDown == 0)
	{
		pointerEventData.pointerPress = nullptr;
	}

	// OnDrag
	if (pInput->MouseFlag() & 0x0001 && prevMouseFlag != 0x0001)
	{
		for (auto i : onDrag)
		{
			i.second->Prepare(i.first, &pointerEventData);
		}
	}

	int nObjectGetDrag = 0;
	if (pInput->MouseFlag() & 0x0001 ||
		(pInput->MouseFlag() & 0x0001) && prevMouseFlag & 0x0001 ||
		!(pInput->MouseFlag() & 0x0001) && prevMouseFlag & 0x0001)
	{
		for (auto i : onDrag)
		{
			if (i.second->Frame(i.first, &pointerEventData) == 0)
			{
				nObjectGetDrag++;
			}
		}
	}

	// OnDrop
	if (prevMouseFlag == 0x0001 && !(pInput->MouseFlag() & 0x0001))
	{
		for (auto i : onDrop)
		{
			i.second->Frame(i.first, &pointerEventData);
		}
	}

	if (nObjectGetDrag == 0)
	{
		pointerEventData.pointerDrag = nullptr;
	}

	prevMouseFlag = pInput->MouseFlag() & 0x00ff;

	return 0;
}

RESULT EventDistributor::Release()
{
	delete this;
	return 0;
}

RESULT EventDistributor::Remove(Invokable* command)
{
	for (int i = 0; i < 255; i++) {
		UnsubscribeOnKeyDown(i, command);
		UnsubscribeWhileKeyDown(i, command);
		UnsubscribeMouseClick(command);
	}
	return 0;
}
RESULT EventDistributor::SubscribeOnKeyDown(char x, Invokable* command)
{
	onKeyDown[x].insert(command);
	return 0;
}

RESULT EventDistributor::SubscribeWhileKeyDown(char x, Invokable* command)
{
	whileKeyDown[x].insert(command);
	return 0;
}

RESULT EventDistributor::UnsubscribeOnKeyDown(char i, Invokable* command)
{
	onKeyDown[i].erase(command);
	return 0;
}

RESULT EventDistributor::UnsubscribeWhileKeyDown(char i, Invokable* command)
{
	whileKeyDown[i].erase(command);
	return 0;
}

RESULT EventDistributor::SubscribeMouseClick(FUNCTION(void *, check, Point), Invokable* command)
{
	onMouseClick[command] = check;
	return 1;
}

RESULT EventDistributor::UnsubscribeMouseClick(Invokable* command)
{
	onMouseClick.erase(command);
	return 0;
}

RESULT EventDistributor::SubscribePointerDown(GameObject* gameObject, IPointerDownHandler* function)
{
	pointerDown.push_back(std::pair<GameObject*, IPointerDownHandler*>(gameObject,function));
	return 0;
}

RESULT EventDistributor::SubscribeOnDrag(GameObject* gameObject, IDragHandler* function)
{
	onDrag.push_back(std::pair<GameObject*, IDragHandler*>(gameObject, function));
	return 0;
}

RESULT EventDistributor::SubscribeOnDrop(GameObject* gameObject, IDropHandler* function)
{
	onDrop.push_back(std::pair<GameObject*, IDropHandler*>(gameObject, function));
	return 0;
}
