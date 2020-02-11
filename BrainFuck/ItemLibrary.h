#pragma once
#include "useful_stuff.h"
#include "TextureObject.h"
#include <vector>

static const int NO_OF_DIF_ITEM = 10;

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

	// Should call TextureObject::operator =
	virtual Item& operator = (const Item&) = 0;
	virtual RESULT Frame() = 0;
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

