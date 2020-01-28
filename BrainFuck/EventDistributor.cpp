#include "EventDistributor.h"



EventDistributor::EventDistributor()
{
}


EventDistributor::~EventDistributor()
{
}

RESULT EventDistributor::Initialize(Input* input)
{
	this->input = input;
	return 0;
}

RESULT EventDistributor::Frame()
{
	for (char i = 0; i < 256; i++) {
		switch (input->keyboard(i))
		{
		case 3:
			for (auto command : onKeyDown[i])
				command->Invoke(&i);
			// no break;
		case 1:
			for (auto command : whileKeyDown[i])
				command->Invoke(&i);
			break;
		default:
			break;
		}
	}
	if (input->MouseFlag() & 0x0100) {
		for (int i = 0; i < mouseClickCondition.size(); i++)
		{
			void* result = mouseClickCondition[i](input->Mouse());
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
	ERRORCHECK(
	if (mouseClickCondition.size() != onMouseClick.size()) {
		cerr << "EventDistributor Error: wrong array size.";
		return 1;
	}
	);
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
