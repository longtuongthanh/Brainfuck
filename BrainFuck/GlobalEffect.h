#pragma once
#include "useful_stuff.h"
#include "GlobalEffectOverflow.h"
#include "GlobalEffectTurnCycle.h"

class GlobalEffect : public GlobalEffectOverflow, public GlobalEffectTurnCycle
{
public:
	GlobalEffect();
	~GlobalEffect();
	
	// TODO: Add all pre-frame calculation here
	RESULT FramePreCalc(Timer*);
	// TODO: Add all post-frame calculation here
	RESULT FramePostCalc(mydeque<mydeque<HexagonTile*> >&);
};

