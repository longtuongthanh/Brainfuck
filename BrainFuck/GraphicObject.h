#pragma once

#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX10math.h>

class GraphicObject
{
public:
	GraphicObject();
	virtual ~GraphicObject();

	virtual RESULT Release() = 0;
	virtual RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX) = 0;

	virtual RESULT Frame() = 0;
};

