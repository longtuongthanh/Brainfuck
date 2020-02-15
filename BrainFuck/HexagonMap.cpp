#include "HexagonMap.h"


HexagonMap::HexagonMap()
{
	itemLib = 0;
}

HexagonMap::~HexagonMap()
{
	DESTROY(itemLib);
}

HexagonMap& HexagonMap::operator=(const HexagonMap& map)
{
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
	/*
	for (auto row : map.value)
		for (auto tile : row.value)
			if (tile != NULL) 
				tile->Frame();
	map.value[0].value[0]->FramePrototype();
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
				int trgx = i + cameraTile.x;
				int trgy = j + cameraTile.y;
				HexagonTileBase*& tile = map[trgx][trgy];
				if (tile == NULL) {
					if (trgx % 10 == 0 && trgy % 10 == 0)
						tile = new TileDetermination(GetLocation(trgx, trgy));
					else
						tile = new HexagonTile(GetLocation(trgx, trgy));
					tile->Initialize(pDevice, pTextureLib, pShaderLib->GetTextureShader(), itemLib);
				}
				tile->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			}
		}
    return 0;
}

RESULT HexagonMap::Initialize(ID3D11Device* device, TextureLibrary* textureLib, ShaderLibrary* shaderLib)
{
    this->pDevice = device;
    this->pTextureLib = textureLib;
    this->pShaderLib = shaderLib;

	BLOCKALLOC(ItemLibrary, itemLib);
	BLOCKCALL(itemLib->Initialize(device, textureLib, shaderLib->GetTextureShader()), "Cannot initialize item library");

    InitializeData();

    return 0;
}

RESULT HexagonMap::Release()
{
	delete this;
	return 0;
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
				HexagonTileBase* newTile;
				if (i % 10 == 0 && j % 10 == 0)
					newTile = new TileDetermination(GetLocation(i, j));
				else
					newTile = new HexagonTile(GetLocation(i, j));
				newTile->Initialize(pDevice, pTextureLib, pShaderLib->GetTextureShader(), itemLib);
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

Point HexagonMap::GetCoord(Point fieldCoord)
{
	return Point(round((fieldCoord.x) / HEXAGON_SIZE / sqrt(3) * 2 + fieldCoord.y * 2 / HEXAGON_SIZE / 3), 
				 round(fieldCoord.y * 4 / HEXAGON_SIZE / 3));
}

Point HexagonMap::GetLocation(int x, int y)
{
	return Point((x - y / 2.0) * HEXAGON_SIZE * sqrt(3) / 2, 3 * y * HEXAGON_SIZE / 4);
}
