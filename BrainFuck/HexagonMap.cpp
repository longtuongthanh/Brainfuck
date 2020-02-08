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

HRESULT HexagonMap::Frame(const Point& cameraPos)
{
    // if camera move outside of all tile, then create new one 
	// (MOVED TO RENDER) Reason: Frame is for all frame calculation.

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
    // */
    /*    END OF CHECK   */

	// TODO: Stays until add Fustrum Culling
	// current largest source of lag
	//*
	for (auto row : map.value)
		for (auto tile : row.value)
			if (tile != NULL) 
				tile->Frame();
	// */

    return 0;
}

HRESULT HexagonMap::Render(Point cameraPos,
							ID3D11DeviceContext* deviceContext, 
                            D3DXMATRIX worldMatrix,
                            D3DXMATRIX viewMatrix,
                            D3DXMATRIX projectionMatrix)
{
	Point cameraTile = GetCoord(cameraPos);
	for (int i = min_X; i <= max_X; i++)
		for (int j = min_Y; j <= max_Y; j++)
		{
			Point pos = GetLocation(i, j);
			if (pos.x > -1.2 && pos.x < 1.2 && pos.y > -1.2 && pos.y < 1.2)
			{
				HexagonTile*& tile = map[i + cameraTile.x][j + cameraTile.y];
				if (tile == NULL)
					tile = NewHexagonTile(i + cameraTile.x, j + cameraTile.y, 
										  tileWidth, tileHeight, padding);
				tile->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			}
		}
    return 0;
}

RESULT HexagonMap::Initialize(ID3D11Device* pDevice, TextureLibrary* textureLib, ShaderLibrary* pShaderLib)
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

    max_X = 12;
    min_X = -12;
	max_Y = 7;
	min_Y = -7;

    for (int i = min_X; i <= max_X; i++)
        for (int j = min_Y; j <= max_X; j++)
			if (i - j >= min_X && i - j <= max_X)
			{				
            // this is a test, in real game we will load data from file
				HexagonTile* newTile = NewHexagonTile(i, j, tileWidth, tileHeight, padding);
				map[i][j] = newTile;
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
    HexagonTile* newTile = new HexagonTile(Point((xCoord - yCoord / 2.0) * tileWidth, 3 * yCoord * tileHeight / 4), tileWidth - padding, tileHeight - padding);

    newTile->Initialize(pDevice, TEXTURE_FILE, textureLib, pShaderLib->GetTextureShader());
    //map.push_back(newTile);
    return newTile;
}

Point HexagonMap::GetCoord(Point fieldCoord)
{
	return Point(round((fieldCoord.x) / tileWidth + fieldCoord.y * 2 / tileHeight / 3), 
				 round(fieldCoord.y * 4 / tileHeight / 3));
}

Point HexagonMap::GetLocation(int x, int y)
{
	return Point((x - y / 2.0) * tileWidth, 3 * y * tileHeight / 4);
}
