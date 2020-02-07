#include "EventDistributor.h"



EventDistributor::EventDistributor()
{
	locked = false;
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
	if (!locked) return 1;
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
		for (int i = 0; i < mouseClickCondition.size(); i++)
		{
			void* result = mouseClickCondition[i](pInput->MouseToScreen());
			if (result != NULL)
				onMouseClick[i]->Invoke(result);
		}
	}
	return 0;
}

RESULT EventDistributor::Release()
{
	delete this;
	return 0;
}

RESULT EventDistributor::Lock()
{
	for (int i = 0; i < 256; i++) {
		onKeyDown[i].shrink_to_fit();
		whileKeyDown[i].shrink_to_fit();
	}
	mouseClickCondition.shrink_to_fit();
	onMouseClick.shrink_to_fit();
	DEBUG(
	if (mouseClickCondition.size() != onMouseClick.size()) {
		cerr << "EventDistributor Error: wrong array size.";
		return 1;
	}
	);
	// end DEBUG
	locked = true;
	return 0;
}

RESULT EventDistributor::SubscribeOnKeyDown(char x, Invokable* command)
{
	if (!locked) {
		onKeyDown[x].push_back(command);
		return 0;
	}
	else return 1;
}

RESULT EventDistributor::SubscribeWhileKeyDown(char x, Invokable* command)
{
	if (!locked) {
		whileKeyDown[x].push_back(command);
		return 0;
	}
	else return 1;
}

RESULT EventDistributor::SubscribeMouseClick(FUNCTION(void *, check, Point), Invokable* command)
{
	if (locked) {
		mouseClickCondition.push_back(check);
		onMouseClick.push_back(command);
		return 0;
	}
	return 1;
}
