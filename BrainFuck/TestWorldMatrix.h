#pragma once
#include "TextureObject.h"
#include "useful_stuff.h"

class TestDragable:public TextureObject, public DragAndDrop
{
public:
    TestDragable();

public:
    void MakeWorldMatrix()
    {
        D3DXMatrixIdentity(pMatWorld);
        D3DXMatrixTranslation(pMatWorld, position.x, position.y, 0);
    }

    RESULT InitializeData();
    RESULT Frame(Input& input, Point camPos);

private:
    std::vector<Point> GetTransformedHitbox();

public:
    Point position;
    D3DXMATRIX* pMatWorld;
    std::vector<Point> hitBox;
};