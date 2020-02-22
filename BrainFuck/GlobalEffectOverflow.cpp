#include "GlobalEffectOverflow.h"
#include "HexagonTile.h"

GlobalEffectOverflow::GlobalEffectOverflow() : n(), memory()
{
}

GlobalEffectOverflow::~GlobalEffectOverflow()
{
}

RESULT GlobalEffectOverflow::OverflowReport(int x, int y, int typeID)
{
	if (n >= OVERFLOW_RESULUTION_PER_FRAME) {
		DEBUG(cerr << "OVERFLOW");
		return 1;
	}
	memory[n] = std::make_tuple(x, y, typeID);
	n++;
	return 0;
}

RESULT GlobalEffectOverflow::OverflowResolve(mydeque<mydeque<HexagonTile*>>& map)
{
	for (int i = 0; i < n; i++) {
		int& x = std::get<0>(memory[i]);
		int& y = std::get<1>(memory[i]);
		int& typeID = std::get<2>(memory[i]);
		DEBUG(
			if (!PushAttempt(map[x][y], typeID)) {
				cerr << "WARNING: false overflow report - tile can still contain target\n";
				return 0;
			}
		)
		Coord seed = Coord(x, y) + dir[4];
		CircularIteration(seed, map, typeID);
	}
	n = 0;
	return 0;
}

RESULT GlobalEffectOverflow::PushAttempt(HexagonTile* tile, int typeID)
{
	if (tile == NULL) return 1;
	return tile->AddItem(typeID);
}

RESULT GlobalEffectOverflow::CircularIteration(Coord seed, mydeque<mydeque<HexagonTile*>>& map, int typeID)
{
	for (int i = 1; true; i++) {
		Coord cur = seed;
		for (int d = 0; d < 6; d++)
			for (int j = 0; j < i; j++) {
				if (PushAttempt(map[cur.x][cur.y], typeID) == 0)
					return 0;
				cur += dir[d];
			}
		seed += dir[4];
		DEBUG(
			if (i > 5) {
				cerr << "WARNING: Overflow attempt over level 5, stopping.\n";
				return 1;
			}
		)
	}
}
