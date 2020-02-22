#pragma once
#include "useful_stuff.h"
#include "Timer.h"
static const float TIME_BETWEEN_TURN = 1.0;
class GlobalEffectTurnCycle
{
public:
	GlobalEffectTurnCycle();
	~GlobalEffectTurnCycle();

	bool IsNewTurn();
	RESULT NewFrame(Timer*);
private:
	float timeElapsed;
	bool turn;
};

