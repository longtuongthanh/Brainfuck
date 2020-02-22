#include "GlobalEffect.h"



GlobalEffect::GlobalEffect()
{
}


GlobalEffect::~GlobalEffect()
{
}

RESULT GlobalEffect::FramePreCalc(Timer* frameTimer)
{
	BLOCKCALL(NewFrame(frameTimer),
		"turn check fail");
	return 0;
}

RESULT GlobalEffect::FramePostCalc(mydeque<mydeque<HexagonTile*>>& map)
{
	BLOCKCALL(OverflowResolve(map),
		"cannot resolve overflow.");
	return 0;
}
