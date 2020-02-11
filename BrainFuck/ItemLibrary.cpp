#include "ItemLibrary.h"



ItemLibrary::ItemLibrary()
{
}


ItemLibrary::~ItemLibrary()
{
}

Item * ItemLibrary::GetPrototype(int id) { 
	DEBUG(
		if (prototype[id] == NULL) {
			cerr << "ERROR: ITEM NOT AVAILABLE.";
		}
	)
	return prototype[id]; 
}

RESULT ItemLibrary::Initialize(ID3D11Device *, TextureLibrary *, TextureShader *)
{
	throw OSS_UNIMPLEMENTED;
	return 0;
}

RESULT ItemLibrary::Release()
{
	for (auto item : prototype)
		DESTROY(item);
	delete this;
	return 0;
}

RESULT ItemLibrary::AddItem(Item * item) {
	int id = item->id();
	if (prototype[id]) {
		cerr << "WARNING: ID COLLISION";
		return 1;
	}
	else
		prototype[id] = item;
	return 0;
}
