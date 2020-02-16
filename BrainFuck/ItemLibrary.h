#pragma once
#include "useful_stuff.h"
#include "TextureObject.h"
#include <vector>
#include "game_constants.h"

class Item : public TextureObject {
	//static const char* name;
public:
	Item() {};
	virtual ~Item() {};
	// DO NOT OVERRIDE. Overide operator = instead.
	Item(const Item& x) {
		*this = x;
	};

	virtual int id() = 0;
	virtual const char* Name() = 0;
	// {return name;}

	// Should call TextureObject::operator =(*this, other)
	virtual Item& operator = (const Item&) = 0;
	virtual RESULT Frame() = 0;
	virtual RESULT FrameBehaviour();
	RESULT InitializeData() final;
	RESULT Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	D3DXMATRIX WorldMatrix(const D3DXMATRIX&, int);
	/*
	Hexagon side considered 1.
	Postype:
		1
	2		6
		0
	3		5
		4
	*/
	static Point Offset(int postype);
};

class ItemLibrary : NonCopyable
{
	Item* prototype[NO_OF_DIF_ITEM];
public:
	ItemLibrary();
	~ItemLibrary();

	// TODO: ADD ANY ITEM OBJECT HERE
	RESULT Initialize(ID3D11Device*, TextureLibrary*, TextureShader*);
	RESULT Release();
	
	RESULT AddItem(Item* item);

	Item* GetPrototype(int id);
};

static const char* ITEM_WILL_FILE = "will.dds";
class ItemWill final : public Item {
	int id();
	const char* Name();

	Item& operator = (const Item&);
	RESULT Frame();
};