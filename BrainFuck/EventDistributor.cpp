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

RESULT EventDistributor::Frame()
{
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
