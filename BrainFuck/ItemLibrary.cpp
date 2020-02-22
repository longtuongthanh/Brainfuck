#include "ItemLibrary.h"

TextString* Item::text = 0;

ItemLibrary::ItemLibrary(): prototype()
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

RESULT ItemLibrary::Initialize(ID3D11Device* device, TextureLibrary* textureLib, ShaderLibrary* shaderLib)
{
	BLOCKALLOC(TextString, Item::text);
	Item::text->Initialize(device, 1, textureLib, shaderLib->GetFontShader());
	Item::text->size = 0.1;
	Item* will;
	BLOCKALLOC(ItemWill, will);
	BLOCKCALL(will->Initialize(device, ITEM_WILL_FILE, textureLib, shaderLib->GetTextureShader()),
		"Cannot initialize Will");
	AddItem(will);

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

RESULT Item::FrameBehaviour()
{
	return 0;
}
RESULT Item::InitializeData()
{
	pointCount = 4;
	BLOCKALLOC(VertexType[pointCount], pointArray);

	(*this)[0].position = D3DXVECTOR3(-ITEM_SIZE / 2, -ITEM_SIZE / 2, 1);
	(*this)[1].position = D3DXVECTOR3(ITEM_SIZE / 2, -ITEM_SIZE / 2, 1);
	(*this)[2].position = D3DXVECTOR3(ITEM_SIZE / 2, ITEM_SIZE / 2, 1);
	(*this)[3].position = D3DXVECTOR3(-ITEM_SIZE / 2, ITEM_SIZE / 2, 1);

	(*this)[0].texture = D3DXVECTOR2(0, 0);
	(*this)[1].texture = D3DXVECTOR2(1, 0);
	(*this)[2].texture = D3DXVECTOR2(1, 1);
	(*this)[3].texture = D3DXVECTOR2(0, 1);

	return 0;
}

D3DXMATRIX Item::WorldMatrix(const D3DXMATRIX& worldMatrixOriginal, int offsetType)
{
	D3DXMATRIX newMatrix;
	Point offset = Offset(offsetType) ;
	D3DXMatrixTranslation(&newMatrix, offset.x, offset.y, 0);
	return newMatrix * worldMatrixOriginal;
}

RESULT Item::Render(ID3D11DeviceContext* deviceContext, 
					int offsetType,
					D3DXMATRIX worldMatrix, 
					D3DXMATRIX viewMatrix, 
					D3DXMATRIX projectionMatrix,
					int text)
{
	D3DXMATRIX newWorld = WorldMatrix(worldMatrix, offsetType);
	BLOCKCALL(__super::Render(deviceContext, 
							  newWorld, 
							  viewMatrix, projectionMatrix),
		"can't render item\n");
	this->text->InputString(std::to_string(text));
	BLOCKCALL(this->text->Render(deviceContext,
						  newWorld,
						  viewMatrix, projectionMatrix),
		"can't render item count");
	return 0;
}

Point Item::Offset(int postype)
{
	switch (postype) {
	case 0:
		return Point(0, 0);
	case 1:
		return Point(0, OFFSET_DISTANCE);
	case 2:
		return Point(-sqrt(3) * OFFSET_DISTANCE / 2, OFFSET_DISTANCE / 2);
	case 3:
		return Point(-sqrt(3) * OFFSET_DISTANCE / 2, -OFFSET_DISTANCE / 2);
	case 4:
		return Point(0, -OFFSET_DISTANCE);
	case 5:
		return Point(sqrt(3) * OFFSET_DISTANCE / 2, -OFFSET_DISTANCE / 2);
	case 6:
		return Point(sqrt(3) * OFFSET_DISTANCE / 2, OFFSET_DISTANCE / 2);
	default:
		DEBUG(cerr << "wrong position type: " << postype;)
	}
}

int ItemWill::id()
{
	return ITEM_WILL_ID;
}

const char * ItemWill::Name()
{
	return "Will";
}

Item & ItemWill::operator=(const Item &other)
{
	TextureObject::operator=(other);
	return *this;
}

RESULT ItemWill::Frame()
{
	return 0;
}
