#pragma once

#include <tuple>
#include "useful_stuff.h"
#include "mydeque.h"

static const int OVERFLOW_RESULUTION_PER_FRAME = 10;
class GlobalEffectOverflow
{
public:
	GlobalEffectOverflow();
	virtual ~GlobalEffectOverflow();

	RESULT OverflowReport(int x, int y, int typeID);
	RESULT OverflowResolve(mydeque<mydeque<HexagonTile*> >&);
private:
	// int x, int y, int typeID, int amount
	std::tuple<int, int, int> memory[OVERFLOW_RESULUTION_PER_FRAME];
	RESULT PushAttempt(HexagonTile*, int);
	RESULT CircularIteration(Coord, mydeque<mydeque<HexagonTile*>>&, int);
	int n;
};

