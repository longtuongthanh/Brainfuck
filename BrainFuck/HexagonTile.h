#pragma once

#include "TextureObject.h"

class HexagonTile: public TextureObject
{
private:
	class Storage
	{
	public:
		Storage();
		Storage(const Storage&);
		~Storage();

	private:
		// store some information here
	};

public:
	HexagonTile();
	HexagonTile(Point position, FLOAT width, FLOAT height);
	HexagonTile(const HexagonTile&);
	~HexagonTile();

protected:
	RESULT InitializeData(); // We can use this to initialize data storage or behavior

private:
	Storage storage; // Storage for tile
	Point position; 
	FLOAT width, height;
	// Maybe we could add Behavior class that handle some behaviors for the brain-cell
};