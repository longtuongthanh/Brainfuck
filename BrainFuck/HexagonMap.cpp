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

HRESULT HexagonMap::Frame(CameraClass* camera)
{
    // if camera move outside of all tile, then create new one 
    if (camera->position.x + 1 > max_X * tileWidth)
    {
        int numberOfNewTile = (camera->position.x + 1 - max_X * tileWidth) / tileWidth + 1;
        for (int x = max_X + 1; x <= max_X + numberOfNewTile; x++)
        {
            map.insert(map.end(), std::vector<HexagonTile*>());
            for (int y = min_Y; y <= max_Y; y++)
            {
                HexagonTile* newTile = NewHexagonTile(x, y, tileWidth, tileHeight, padding);
                map.back().push_back(newTile);
            }
        }
        max_X += numberOfNewTile;
    }

    if (camera->position.y + 1 > 3 * max_Y * tileHeight / 4)
    {
        int numberOfNewTile = (camera->position.y + 1 - 3 * max_Y * tileHeight / 4) / tileHeight + 1;
        for (int x = min_X; x <= max_X; x++)
        {
            for (int y = max_Y + 1; y <= max_Y + numberOfNewTile; y++)
            {
                HexagonTile* newTile = NewHexagonTile(x, y, tileWidth, tileHeight, padding);
                map[x-min_X].push_back(newTile);
            }
        }
        max_Y += numberOfNewTile;
    }

    if (camera->position.x - 1 < min_X * tileWidth)
    {
        int numberOfNewTile = -((camera->position.x - 1 - min_X * tileWidth) / tileWidth - 1);
        for (int x = min_X - 1; x >= min_X - numberOfNewTile; x--)
        {
            map.insert(map.begin(), std::vector<HexagonTile*>());
            for (int y = min_Y; y <= max_Y; y++)
            {
                HexagonTile* newTile = NewHexagonTile(x, y, tileWidth, tileHeight, padding);
                map.front().push_back(newTile);
            }
        }
        min_X -= numberOfNewTile;
    }

    if (camera->position.y - 1 < 3 * min_Y * tileHeight / 4)
    {
        int numberOfNewTile = -((camera->position.y - 1 - 3 * min_Y * tileHeight / 4) / tileHeight - 1);
        for (int x = min_X; x <= max_X; x++)
        {
            for (int y = min_Y - 1; y >= min_Y - numberOfNewTile; y--)
            {
                HexagonTile* newTile = NewHexagonTile(x, y, tileWidth, tileHeight, padding);
                map[x-min_X].push_back(newTile);
            }
        }
        min_Y -= numberOfNewTile;
    }

    /*           DO NOT USE THIS FOR REAL GAMEPLAY         */
    // This code check if there is any dupplicated tile at one position
    // Use for debug only, it burns FPS real fast
    /*
    for (auto vector : map)
    {
        for (auto tile : vector)
        {
            for (auto vector2 : map)
            {
                for (auto tile2 : vector2)
                {
                    if (tile != tile2 && tile->GetPosition() == tile2->GetPosition())
                    {
                        return 1;
                    }
                }
            }
        }
    }
    */
    /*    END OF CHECK   */

    return 0;
}

HRESULT HexagonMap::Render(ID3D11DeviceContext* deviceContext, 
                            D3DXMATRIX worldMatrix,
                            D3DXMATRIX viewMatrix,
                            D3DXMATRIX projectionMatrix)
{
    for(auto vector : map)
    {
        for (auto tile : vector)
        {
            tile->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
        }
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

    max_X = max_Y = 10;
    min_X = min_Y = -10;

    for (int i = min_X; i <= max_X; i++)
    {
        map.push_back(std::vector<HexagonTile*>());
        for (int j = min_Y; j <= max_X; j++)
        {
            // this is a test, in real game we will load data from file
            HexagonTile* newTile = NewHexagonTile(i, j, tileWidth, tileHeight, padding);
            map.back().push_back(newTile);
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

HexagonTile* &HexagonMap::NewHexagonTile(INT xCoord, INT yCoord, FLOAT tileWidth, FLOAT tileHeight, FLOAT padding)
{
    HexagonTile* newTile;
    if (yCoord % 2 == 0)
    {
        newTile = new HexagonTile(Point(xCoord * tileWidth + tileWidth / 2, 3 * yCoord * tileHeight / 4), tileWidth - padding, tileHeight - padding);
    }
    else
    {
        newTile = new HexagonTile(Point(xCoord * tileWidth, 3 * yCoord * tileHeight / 4), tileWidth - padding, tileHeight - padding);
    }

    newTile->Initialize(pDevice, TEXTURE_FILE, textureLib, pShaderLib->GetTextureShader());
    //map.push_back(newTile);
    return newTile;
}
