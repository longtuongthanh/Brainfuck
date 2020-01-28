#pragma once

#undef __in
#include <vector>
#include "useful_stuff.h"
#include "Input.h"

/** Distribute input event to corresponding 
	For events that happen regularly, not one-off event.*/
class EventDistributor
{
public:
	EventDistributor();
	~EventDistributor();

	RESULT Initialize(Input* input);
	/** Events will happen in each frame in the order of subscribtion
		after this is Lock()-ed. Should be called before Input.Frame()*/
	RESULT Frame();
	RESULT Release();

	/** Try not to use this. Removes all pointers with same address 
		by iterating through all of them (UNIMPLEMENTED)*/
	RESULT Remove(Invokable*);
	/** Prevents subscribtion and start allowing events to happen.*/
	RESULT Lock();

	RESULT SubscribeOnKeyDown(char, Invokable*);
	RESULT SubscribeWhileKeyDown(char, Invokable*);
	RESULT SubscribeMouseClick(FUNCTION(void*, check, Point), Invokable*);
private:
	/** Invokable will be passed a pointer containing the character.*/
	std::vector<Invokable*> onKeyDown[256];
	std::vector<Invokable*> whileKeyDown[256];
	/** The corresponding onMouseClick Invokable will be called 
		with parameter returned by mouseClickCondition. Return null
		if you dont want onMouseClick to be called.*/
	std::vector<FUNCTION(void*, , Point)> mouseClickCondition;
	std::vector<Invokable*> onMouseClick;
	
	Input* input;
	bool locked;
};

