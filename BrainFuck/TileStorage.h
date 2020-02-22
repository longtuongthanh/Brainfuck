#pragma once

#include <bitset>
#include "ItemLibrary.h"

/*
	1
2		6
	0
3		5
	4
*/
static int OFFSET_TYPE[7][7] = {
	{ 0, -1, -1, -1, -1, -1, -1},
	{ 1, 4, -1, -1, -1, -1, -1},
	{ 2, 6, 4, -1, -1, -1, -1},
	{ 2, 6, 4, 0, -1, -1, -1},
	{ 2, 6, 3, 5, 0, -1, -1},
	{ 1, 2, 6, 3, 5, 4, -1},
	{ 1, 2, 6, 3, 5, 4, 0},
};

class TileStorage
{
public:
	TileStorage();
	RESULT InitializeData(unsigned char types, int typeIDs...);
	TileStorage(const TileStorage& x);
	~TileStorage();

	RESULT Initialize(ItemLibrary*);
	RESULT Frame();
	RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	/** Return offsetType of the item at [current] if there are <types> item types.*/
	static int offsetType(int, int);
	RESULT AddItem(unsigned char);
	// store some information here
	std::bitset<7> any;
	unsigned char typeID[7];
	unsigned char amount[7];
	unsigned char ntypes;
private:
	ItemLibrary* itemLib;
	/*
		There are a max of 7 types in 1 tile.
		Location based on number of types:
		1:
			0
		0		0
			X
		0		0
			0
		2:
			X
		0		0
		0		0
			X
		3:
			0
		X		X
		0		0
			X
		4:
			0
		X		X
			X
		0		0
			X
		5:
			0
		X		X
			X
		X		X
			0
	*/
};