#include "GardenState.h"
#include "WindowPainter.h"
// include weather engine
#pragma once

class WorldManager
{
public:
	WorldManager(int,int, int);
	~WorldManager();
	GardenState* garden;
	WindowPainter* wPainter;
	//Weather Engine

	//May implement a method to push environment state updates to the garden
	//PerformEngienUpdate();
	bool RenderScene();

};
