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
    this->tileWidth = map.tileWidth;
    this->tileHeight = map.tileHeight;
    this->padding = map.padding;
    return *this;
}

HRESULT HexagonMap::Frame()
{
    return 0;
}

HRESULT HexagonMap::Render(ID3D11DeviceContext* deviceContext, 
                            D3DXMATRIX worldMatrix,
                            D3DXMATRIX viewMatrix,
                            D3DXMATRIX projectionMatrix)
{
    for(auto tile : map)
    {
        tile->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    }
    return 0;
}

RESULT HexagonMap::Initialize(ID3D11Device* pDevice, TextureClass* textureLib, ShaderLibrary* pShaderLib)
{
    this->pDevice = pDevice;
    this->textureLib = textureLib;
    this->pShaderLib = pShaderLib;

    InitializeData();

    return RESULT();
}

RESULT HexagonMap::InitializeData()
{
    FLOAT xCenter, yCenter, zCenter;

    xCenter = 0.0f;
    yCenter = 0.0f;
    zCenter = 10.0f;

    for (int i = -20; i < 20; i++)
    {
        for (int j = -20; j < 20; j++)
        {
            NewHexagonTile(i, j, tileWidth, tileHeight, padding);
        }
    }
    return 0;
}

RESULT HexagonMap::AddHexagon(FLOAT xCenter, FLOAT yCenter, FLOAT zCenter, FLOAT width, FLOAT height)
{
    /*
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
    */

    return 0;
}

HexagonTile* HexagonMap::NewHexagonTile(INT xCoord, INT yCoord, FLOAT tileWidth, FLOAT tileHeight, FLOAT padding)
{
    HexagonTile* newTile;
    if (yCoord % 2 == 0)
    {
        newTile = new HexagonTile(Point(xCoord * tileWidth + tileWidth / 2, 1.5 * yCoord * tileHeight / 2), tileWidth - padding, tileHeight - padding);
    }
    else
    {
        newTile = new HexagonTile(Point(xCoord * tileWidth, 1.5 * yCoord * tileHeight / 2), tileWidth - padding, tileHeight - padding);
    }

    newTile->Initialize(pDevice, TEXTURE_FILE, textureLib, pShaderLib->GetTextureShader());
    map.push_back(newTile);
    return newTile;
}
