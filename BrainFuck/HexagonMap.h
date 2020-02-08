#pragma once
#include "TextureObject.h"
#include "useful_stuff.h"
#include "HexagonTile.h"
#include <vector>
#include <deque>
#include "ShaderLibrary.h"

static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";
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
	void cutoff_front(int front) {
		while (front - offset < 0) {
			offset++;
			value.pop_front();
		}
	}
	void cutoff_back(int back) {
		while (back - offset >= value.size())
			value.pop_back();
	}
	mydeque(int offset = 0) : 
		offset(offset) {}
	~mydeque() {}
};

class HexagonMap
{
public:
	HexagonMap();
	HexagonMap(FLOAT tileWidth, FLOAT tileHeight, FLOAT padding);
	~HexagonMap();

	HexagonMap& operator=(const HexagonMap &);
	HRESULT Frame(const Point&);

	HRESULT Render(Point, ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	RESULT Initialize(ID3D11Device*, TextureLibrary*, ShaderLibrary*);

	/** Get the coordinate of the hexagon that contains this point.*/
	Point GetCoord(Point);
	/** Return the center of the hexagon at that coordinate*/
	Point GetLocation(int x, int y);
protected:
	RESULT InitializeData();
	RESULT AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT tileWidth, FLOAT tileHeight);

	HexagonTile* NewHexagonTile(INT xCoord, INT yCoord, FLOAT tileWidth, FLOAT tileHeight, FLOAT padding);

private:
	FLOAT tileWidth, tileHeight; // width and height of hexagon
	FLOAT padding; // space between 2 hexagons

	int max_X, max_Y, min_X, min_Y; // this show the max and min coord of tile

	std::string textureFile;

	mydeque<mydeque<HexagonTile*>> map;

	TextureLibrary* textureLib;
	ShaderLibrary* pShaderLib;
	ID3D11Device* pDevice;

};