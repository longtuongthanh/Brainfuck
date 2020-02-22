#pragma once

#include "TileStorage.h"
#include "game_constants.h"
#include "TileLibrary.h"
#include "TilePrototype.h"

/** Height of hexagon.
	Width of hexagon is sqrt(3) / 2 * SIZE //*/

// Unimplemented, unupdated
class HexagonTileAnimatedPrototype : public HexagonTilePrototype {
	virtual RESULT Frame();
	RESULT Initialize(ID3D11Device*, const CHAR**, int, TextureLibrary*, TextureShader*);
protected:
};
/*
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
//*/
class HexagonTile {
public:
	HexagonTile();
	HexagonTile(Point position, Coord coord, int id = 0);
	HexagonTile(const HexagonTile& x);
	Point GetPosition();

	virtual RESULT Frame(GlobalEffect*);

	virtual RESULT Release();
	virtual RESULT Initialize(ItemLibrary*, TileLibrary*);
	/** ONLY CALL THIS ONCE PER FRAME*/
	RESULT FramePrototype();
	RESULT Render(ID3D11DeviceContext*, D3DXMATRIX , D3DXMATRIX, D3DXMATRIX);

	void WorldMatrix(D3DXMATRIX& worldMatrixOriginal);

	RESULT ChangeType(int id);
	RESULT AddItem(int itemID);

	int GetID();
	TileStorage storage; // Storage for tile
protected:
	Point position;
	Coord coord;
	// Maybe we could add Behavior class that handle some behaviors for the brain-cell
	TileLibrary* tileLib;
	int id;
};

// default tile moved to tilePrototype