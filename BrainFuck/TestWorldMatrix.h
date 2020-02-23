#pragma once
#include "TextureObject.h"
#include "DragAndDrop.h"
#include "IPointerDownHandler.h"
#include "EventDistributor.h"

class TestDragable: public TextureObject, public IPointerDownHandler
{
public:
    TestDragable(EventDistributor*);

public:
    void OnPointerDown();

    void MakeWorldMatrix()
    {
        D3DXMatrixIdentity(pMatWorld);
        D3DXMatrixTranslation(pMatWorld, position.x, position.y, 0);
    }
    virtual RESULT Release() { return 0; };
    virtual RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX) { return 0; };
    RESULT InitializeData();
    RESULT Frame();

private:
    std::vector<Point> GetTransformedHitbox();

public:
    Point position;
    D3DXMATRIX* pMatWorld;
    std::vector<Point> hitBox;
};