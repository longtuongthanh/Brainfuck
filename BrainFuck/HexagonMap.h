#pragma once
#include "TextureObject.h"
#include "useful_stuff.h"
#include "HexagonTile.h"
#include <vector>
#include <deque>
#include "ShaderLibrary.h"
#include "ItemLibrary.h"

#include "TileDetermination.h"

template <typename T>
class mydeque {
	int offset;
public:
	std::deque<T> value;
	T& operator[](int x) {
		while (x - offset < 0)
		{
			if (value.size() == 0)
				offset = x;
			else
				offset--;
			value.push_front(T());
		}
		while (x - offset >= value.size())
		{
			if (value.size() == 0)
				offset = x;
			value.push_back(T());
		}
		return value[x - offset];
	}
	void pop_front() {
		offset++;
		value.pop_front();
	}
	void pop_back() { value.pop_back(); }
	void cutoff_front(int front) {
		while (front - offset < 0)
			pop_front();
	}
	void cutoff_back(int back) {
		while (back - offset >= value.size())
			pop_back();
	}
	mydeque(int offset = 0) : 
		offset(offset) {}
	~mydeque() {}
};

class HexagonMap
{
public:
	HexagonMap();
	~HexagonMap();

	HexagonMap& operator=(const HexagonMap &);
	HRESULT Frame(const Point&);

	HRESULT Render(Point, ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	RESULT Initialize(ID3D11Device*, TextureLibrary*, ShaderLibrary*);
	RESULT Release();

	/** Get the coordinate of the hexagon that contains this point.*/
	Point GetCoord(Point);
	/** Return the center of the hexagon at that coordinate*/
	Point GetLocation(int x, int y);
protected:
	RESULT InitializeData();
	RESULT AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT tileWidth, FLOAT tileHeight);

private:
	int max_X, max_Y, min_X, min_Y; // this show the max and min coord of tile

	std::string textureFile;

	mydeque<mydeque<HexagonTileBase*>> map;

	TextureLibrary* pTextureLib;
	ShaderLibrary* pShaderLib;
	ID3D11Device* pDevice;
	ItemLibrary* itemLib;
};