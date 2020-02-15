#pragma once
#include "HexagonTile.h"

static const CHAR* DETERMINATION_TEXTURE_FILE = "texture.dds";
class TileDetermination : public HexagonTileMiddle<1> {
public:
	TileDetermination();
	TileDetermination(Point position);
	RESULT Release();
	RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*, ItemLibrary*);

	virtual RESULT Frame();
};