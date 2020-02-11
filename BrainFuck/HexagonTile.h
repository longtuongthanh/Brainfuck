#pragma once

#include "TextureObject.h"

/** Height of hexagon.
	Width of hexagon is sqrt(3) / 2 * SIZE //*/
static const double HEXAGON_SIZE = 0.15;
static const double HEXAGON_PADDING = 0.01;

//singleton for rendering
// Its called Flyweight, not prototype... but I like the name prototype better.
class HexagonTilePrototype: public TextureObject, NonCopyable
{
public:
	HexagonTilePrototype() {}
	~HexagonTilePrototype() {}
protected:
	RESULT InitializeData(); // We can use this to initialize data storage or behavior
};

// Unimplemented
class HexagonTileAnimatedPrototype : public HexagonTilePrototype {
	virtual RESULT Frame();
	RESULT Initialize(ID3D11Device*, const CHAR**, int, TextureLibrary*, TextureShader*);
protected:
};

class TileStorage
{
public:
	TileStorage();
	TileStorage(const TileStorage& x);
	~TileStorage();

private:
	// store some information here
	int typeID[6];
	int amount[6];
	int types;
	/*
		There are a max of 6 types in 1 tile.
		Location based on number of types:
		1:
			X
		0		0
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
		X		X
			0
		5:
			0
		X		X
		X		X
			X
	*/
};

// Templated base class
// this allows static members to be different accross classes with different id
// the singleton (HexagonTilePrototype) need to be different to accomodate different pictures.
// C++ disallows static virtual functions/members
// template class need to be in header.
template <int id>
class HexagonTileBase {
public:
	HexagonTileBase(){}
	HexagonTileBase(Point position) : position(position){}
	HexagonTileBase(const HexagonTileBase& x) : position(x.position), storage(x.storage){}
	Point GetPosition() { return position; }

	virtual RESULT Frame() { return 0; }
	/** ONLY CALL THIS ONCE*/
	virtual RESULT Release() = 0;
	/** ONLY CALL THIS ONCE*/
	virtual RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*) = 0;
	/** ONLY CALL THIS ONCE PER FRAME*/
	static RESULT FramePrototype() { return prototype->Frame(); }
	RESULT Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
				  D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
		return prototype->Render(deviceContext, WorldMatrix(worldMatrix), viewMatrix, projectionMatrix);
	}

	D3DXMATRIX WorldMatrix(D3DXMATRIX worldMatrixOriginal)
	{
		D3DXMatrixTranslation(&worldMatrixOriginal, position.x, position.y, 0);
		return worldMatrixOriginal;
	}

protected:
	TileStorage storage; // Storage for tile
	Point position;
	// Maybe we could add Behavior class that handle some behaviors for the brain-cell
	static HexagonTilePrototype* prototype;
};
template<int id> HexagonTilePrototype* HexagonTileBase<id>::prototype(0);

static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";
class HexagonTile : public HexagonTileBase<0> {
public:
	HexagonTile();
	HexagonTile(Point position);
	RESULT Release();
	RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*);
};