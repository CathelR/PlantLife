#include <SDL3/SDL.h>
#include "GardenState.h"
#include "PerlinEngine.h"
#include <random>
#pragma once



//Class member functions
/*===================================================================================================================*/

GardenState::GardenState(int screenX, int screenY, int tileSize)
{
	this->tileSize = tileSize;
	this->screenSizeX = screenX;
	this->screenSizeY = screenY;
	this->sun.screenPos = { 0,0 };
	GenerateGrid();
	GeneratePlants();
}

GardenState::~GardenState()
{
	//Need to clear down plants.
	//If I set each plant up as a class, I can have it delete its own resources, then gharden destructor just calles delete on plants
}


void GardenState::GenerateGrid()
{
	PerlinEngine perlinEngine;
	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			this->grid[x][y].baseCoord.x = x;
			this->grid[x][y].baseCoord.y = y;
			this->grid[x][y].screenCoord = ScreenSpaceConvert(this->grid[x][y].baseCoord);
			//We interpolate from [0->gridSize] to [0->4] for our sample point
			float lerpedY = ((float)4 / (float)gridSize) * (float)y;
			float lerpedX = ((float)4 / (float)gridSize) * (float)x;
			this->grid[x][y].height = perlinEngine.Noise(lerpedX, lerpedY);
			this->grid[x][y].screenCoord.y += (this->grid[x][y].height*15) + 3*(float)x + 3*(float)y;
			this->grid[x][y].centerPos.x = this->grid[x][y].screenCoord.x + this->tileSize / 2;
			this->grid[x][y].centerPos.y = this->grid[x][y].screenCoord.y + this->tileSize / 4;
		}
	}
}

//Possibly convert PLant to a class and use its destructor to cleanup ths branches/beziersegs etc.
bool GardenState::GeneratePlants()
{
	GridSquare* gS = GetGridSquare(5, 5);
	Plant* plant = new Plant;
	plant->rootPos.x = gS->centerPos.x;
	plant->rootPos.y = gS->centerPos.y;
	plant->main = new Branch;
	plant->numRoots = 2;
	plant->main->bSegs = new BezierSeg[2];
	plant->main->bSegs[0] = { gS->centerPos.x,gS->centerPos.y,gS->centerPos.x-20,gS->centerPos.y - 50,gS->centerPos.x,gS->centerPos.y - 100 };
	plant->main->bSegs[1] = { gS->centerPos.x,gS->centerPos.y-100,gS->centerPos.x + 20,gS->centerPos.y - 150,gS->centerPos.x,gS->centerPos.y - 200 };
	plant->main->parent = nullptr;
	//plant->main->defaultPos This would be the starting angle, so either I work that out or thas what defines the seg - would prefer to transition
	//to using polar coords here
	this->plants = plant;
	return true;
}

SDL_FPoint GardenState::ScreenSpaceConvert(SDL_FPoint baseCoord)
{
	SDL_FPoint convPoint;
	convPoint.x = ((baseCoord.x * this->tileSize /2) - (baseCoord.y * this->tileSize /2))+ (screenSizeX/(float)2)-32;
	convPoint.y = ((baseCoord.x * this->tileSize / 4) + (baseCoord.y * this->tileSize / 4))+(screenSizeY/6);
	return convPoint;
}

int GardenState::GetTileSize()
{
	return this->tileSize;
}

GridSquare* GardenState::GetGridSquare(int x, int y)
{
	return &(this->grid[x][y]);
}

