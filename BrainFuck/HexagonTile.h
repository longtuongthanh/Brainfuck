#pragma once

#include "TextureObject.h"
#include "TileStorage.h"
#include "game_constants.h"

/** Height of hexagon.
	Width of hexagon is sqrt(3) / 2 * SIZE //*/

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

class HexagonTileBase {
public:
	HexagonTileBase() {}
	virtual Point GetPosition() = 0;
	virtual RESULT Frame() = 0;
	virtual RESULT Release() = 0;
	virtual RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*, ItemLibrary*) = 0;
	virtual RESULT FramePrototype() = 0;
	virtual RESULT Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) = 0;
};

// Templated base class
// this allows static members to be different accross classes with different id
// the singleton (HexagonTilePrototype) need to be different to accomodate different pictures.
// C++ disallows static virtual functions/members
// template class need to be in header.
template <int id>
class HexagonTileMiddle : public HexagonTileBase{
public:
	HexagonTileMiddle(){}
	HexagonTileMiddle(Point position) : position(position){}
	HexagonTileMiddle(const HexagonTileMiddle& x) : position(x.position), storage(x.storage){}
	Point GetPosition() { return position; }

	virtual RESULT Frame() { return 0; }

	/** ONLY CALL THIS ONCE*/
	virtual RESULT Release() = 0;
	/** ONLY CALL THIS ONCE*/
	virtual RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*, ItemLibrary* itemLib) {
		BLOCKCALL(storage.Initialize(itemLib), "cannot initialize tile storage");
		return 0;
	}
	/** ONLY CALL THIS ONCE PER FRAME*/
	RESULT FramePrototype() { return prototype->Frame(); }
	RESULT Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
				  D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
		WorldMatrix(worldMatrix);
		BLOCKCALL(prototype->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix),
			"cannot render prototype of tile");
		BLOCKCALL(storage.Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix), 
			"cannot render storage.");
		return 0;
	}

	void WorldMatrix(D3DXMATRIX& worldMatrixOriginal)
	{
		D3DXMATRIX newMatrix;
		D3DXMatrixTranslation(&newMatrix, position.x, position.y, 0);
		worldMatrixOriginal *= newMatrix;
	}

protected:
	TileStorage storage; // Storage for tile
	Point position;
	// Maybe we could add Behavior class that handle some behaviors for the brain-cell
	static HexagonTilePrototype* prototype;
};
template<int id> HexagonTilePrototype* HexagonTileMiddle<id>::prototype(0);

static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";
class HexagonTile : public HexagonTileMiddle<0> {
public:
	HexagonTile();
	HexagonTile(Point position);
	RESULT Release();
	RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*, ItemLibrary*);
};
