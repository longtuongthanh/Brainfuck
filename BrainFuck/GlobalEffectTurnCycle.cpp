#include "GlobalEffectTurnCycle.h"



GlobalEffectTurnCycle::GlobalEffectTurnCycle()
{
}


GlobalEffectTurnCycle::~GlobalEffectTurnCycle()
{
}

bool GlobalEffectTurnCycle::IsNewTurn()
{
	return turn;
}

RESULT GlobalEffectTurnCycle::NewFrame(Timer * timer)
{
	timeElapsed += timer->GetTimeSpan();
	if (turn = timeElapsed > TIME_BETWEEN_TURN) {
		timeElapsed -= TIME_BETWEEN_TURN;
		cerr << "newturn\n";
	}
	return 0;
}
