#include "TestWorldMatrix.h"

TestDragable::TestDragable()
{
	pMatWorld = new D3DXMATRIX();
	D3DXMatrixIdentity(pMatWorld);
}

RESULT TestDragable::InitializeData()
{
	pointCount = 3;
	BLOCKALLOC(VertexType[3], pointArray);

	pointArray[0].position = D3DXVECTOR3(-1.0f, 0, 1.0f);
	pointArray[1].position = D3DXVECTOR3(0, 1.0f, 1.0f);
	pointArray[2].position = D3DXVECTOR3(1.0f, 0, 1.0f);

	hitBox.push_back(Point(-1, 0));
	hitBox.push_back(Point(0, 1));
	hitBox.push_back(Point(1, 0));

	return 0;
}

RESULT TestDragable::Frame(Input& input, Point camPos)
{
	UpdateDragAndDrop(position, GetTransformedHitbox(), input, camPos);
	return 0;
}

std::vector<Point> TestDragable::GetTransformedHitbox()
{
	std::vector<Point> output;
	for (auto point : hitBox)
	{
		output.push_back(point + position);
	}
	return output;
}
