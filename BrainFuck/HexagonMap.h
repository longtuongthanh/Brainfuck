#pragma once
#include "TextureObject.h"
#include "useful_stuff.h"
#include "HexagonTile.h"
#include <vector>
#include "ShaderLibrary.h"
#include "CameraClass.h"

static const CHAR* HEXAGON_TEXTURE_FILE = "texture.dds";

class HexagonMap
{
public:
	HexagonMap();
	HexagonMap(FLOAT tileWidth, FLOAT tileHeight, FLOAT padding);
	~HexagonMap();

	HexagonMap& operator=(const HexagonMap &);
	HRESULT Frame(CameraClass* camera);

	HRESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	RESULT Initialize(ID3D11Device*, TextureClass*, ShaderLibrary*);

protected:
	RESULT InitializeData();
	RESULT AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT tileWidth, FLOAT tileHeight);

	HexagonTile* &NewHexagonTile(INT xCoord, INT yCoord, FLOAT tileWidth, FLOAT tileHeight, FLOAT padding);

private:
	FLOAT tileWidth, tileHeight; // width and height of hexagon
	FLOAT padding; // space between 2 hexagons

	int max_X, max_Y, min_X, min_Y; // this show the max and min coord of tile

	std::string textureFile;

	std::vector<std::vector<HexagonTile*>> map;

	TextureClass* textureLib;
	ShaderLibrary* pShaderLib;
	ID3D11Device* pDevice;
};