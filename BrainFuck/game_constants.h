#pragma once

// Tile
static const double HEXAGON_SIZE = 0.15;
static const double HEXAGON_PADDING = 0.01;
static const int NO_OF_DIF_TILE = 10;


// Item
static const int NO_OF_DIF_ITEM = 10;
static const double ITEM_SIZE = 0.025;
static const double OFFSET_DISTANCE = HEXAGON_SIZE * 0.3;

//Tile direction
#include "useful_stuff.h"
static const Coord dir[6] = { 
	Coord(0,1), 
	Coord(-1,0), 
	Coord(-1,-1),  
	Coord(0,-1), 
	Coord(1,0), 
	Coord(1,1) };
/* Direction:
	0		5
1		X		4
	2		3
*/