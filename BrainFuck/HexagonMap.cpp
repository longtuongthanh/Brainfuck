#include "HexagonMap.h"


HexagonMap::HexagonMap()
{

}

HexagonMap::HexagonMap(FLOAT Width, FLOAT Height, FLOAT pad)
{
    this->tileWidth = Width;
    this->tileHeight = Height;
    this->padding = pad;
}

HexagonMap::~HexagonMap()
{
}

HexagonMap& HexagonMap::operator=(const HexagonMap& map)
{
    TextureObject::operator=(map);
    this->tileWidth = map.tileWidth;
    this->tileHeight = map.tileHeight;
    this->padding = map.padding;
    return *this;
}

RESULT HexagonMap::InitializeData()
{
    FLOAT xCenter, yCenter, zCenter;

    xCenter = 0.0f;
    yCenter = 0.0f;
    zCenter = 10.0f;

    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            if (j % 2 == 0)
            {
                AddHexagon(xCenter + i * tileWidth + tileWidth / 2, yCenter + 1.5 * j * tileHeight / 2, zCenter, tileWidth, tileHeight);
            }
            else
            {
                AddHexagon(xCenter + i * tileWidth, yCenter + 1.5 * j * tileHeight / 2, zCenter, tileWidth, tileHeight);
            }

        }
    }
    return 0;
}

RESULT HexagonMap::AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT width, FLOAT height)
{
    width -= 0.01;
    height -= 0.01;
    VertexType* newPointArray;

    int offset;

    D3DXVECTOR3 newVerx[6];
    newVerx[0] = D3DXVECTOR3(xCenter + 0, yCenter + height / 2, zCenter);
    newVerx[1] = D3DXVECTOR3(xCenter + width / 2, yCenter + height / 4, zCenter);
    newVerx[2] = D3DXVECTOR3(xCenter - width / 2, yCenter + height / 4, zCenter);
    newVerx[3] = D3DXVECTOR3(xCenter + width / 2, yCenter - height / 4, zCenter);
    newVerx[4] = D3DXVECTOR3(xCenter - width / 2, yCenter - height / 4, zCenter);
    newVerx[5] = D3DXVECTOR3(xCenter + 0, yCenter - height / 2, zCenter);


    if (pointCount == 0)
    {
        BLOCKALLOC(VertexType[pointCount + 6], newPointArray);
        memcpy(newPointArray, pointArray, pointCount * sizeof(VertexType));

        offset = 0;
    }
    else
    {
        BLOCKALLOC(VertexType[pointCount + 8], newPointArray);
        memcpy(newPointArray, pointArray, pointCount * sizeof(VertexType));

        offset = 2;
        newPointArray[pointCount + 0].position = pointArray[pointCount - 1].position;
        newPointArray[pointCount + 1].position = newVerx[0];
        //newPointArray[pointCount + 2].position = newVerx[0];
    }

    for (int i = 0; i < 6; i++)
    {
        newPointArray[pointCount + offset + i].position = newVerx[i];
    }

    if (pointCount == 0)
    {
        pointCount += 6;
    }
    else
    {
        delete[] pointArray;
        pointCount += 8;
    }

    pointArray = newPointArray;

    return 0;
}
