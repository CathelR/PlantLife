#include "WorldManager.h"
#include "GardenState.h"
#include "WindowPainter.h"
#pragma once

WorldManager::WorldManager(int screenResX, int screenResY, int tileSize)
{
	this->garden = new GardenState(screenResX,screenResY,tileSize);
	this->wPainter = new WindowPainter(screenResX, screenResY, tileSize);
}

WorldManager::~WorldManager()
{
	delete this->garden;
	delete this->wPainter;
}

bool WorldManager::RenderScene()
{
	if (!this->wPainter->PerformRenderCycle(this->garden))
	{
		return false;
	}
}

