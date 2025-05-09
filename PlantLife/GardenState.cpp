#include <SDL3/SDL.h>
#include "GardenState.h"
#include "PerlinEngine.h"
#include <random>
#pragma once

//Static utility function declaration
/*===================================================================================================================*/
static float GetFunctionHeight(int, float, float, int*);
static int* GetFunctionPattern();


//Class member functions
/*===================================================================================================================*/

GardenState::GardenState(int tileSize)
{
	this->tileSize = tileSize;
	this->sun.screenPos = { 0,0 };
	GenerateGrid();
}

GardenState::~GardenState()
{
	
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
			this->grid[x][y].screenCoord.y += (this->grid[x][y].height*10) + 3*(float)x + 3*(float)y;
			this->grid[x][y].centerPos.x = this->grid[x][y].screenCoord.x + this->tileSize / 2;
			this->grid[x][y].centerPos.y = this->grid[x][y].screenCoord.y + this->tileSize / 4;
		}
	}
}

SDL_FPoint GardenState::ScreenSpaceConvert(SDL_FPoint baseCoord)
{
	SDL_FPoint convPoint;
	convPoint.x = ((baseCoord.x * this->tileSize /2) - (baseCoord.y * this->tileSize /2));
	convPoint.y = ((baseCoord.x * this->tileSize / 4) + (baseCoord.y * this->tileSize / 4));
	return convPoint;
}

int GardenState::GetTileSize()
{
	return this->tileSize;
}

GridSquare GardenState::GetGridSquare(int x, int y)
{
	return this->grid[x][y];
}


/*

//This method takes a pattern, and uses this to cyclically "build" a function.
//This basically means that the funciton used to generate the X and Y heights is randomised for each generation, but consistent throughout the generation
static float GetFunctionHeight(int gridPos, float scalar, float modifier, int* pattern)
{
	float height = 0;

	for (int i = 0; i < 3; i++)
	{
		int component = pattern[i];
		switch (component)
		{
		case 1:
			height += scalar * sin(modifier * (float)gridPos);
			break;
		case 2:
			height += scalar * sin(modifier * 2 * (float)gridPos);
			break;
		case 3:
			height += scalar * sin(modifier * 3 * (float)gridPos);
			break;
		case 4:
			height += scalar * cos(modifier * (float)gridPos);
			break;
		case 5:
			height += scalar * cos(modifier * 2 * (float)gridPos);
			break;
		case 6:
			height += scalar * cos(modifier * 3 * (float)gridPos);
			break;

		}

	}
	return height;
}

static int* GetFunctionPattern()
{
	int* funcPattern = new int[3];
	for (int i = 0; i < 3; i++)
	{
		*(funcPattern+i) = SDL_rand(7);
	}
	return funcPattern;
}


{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> scalarDistrib(3, 8);
	std::uniform_int_distribution<> modifierDistrib(1, 4);
	float scalar = scalarDistrib(gen);
	float modifier = modifierDistrib(gen);

	int* xPattern = GetFunctionPattern();
	int* yPattern = GetFunctionPattern();

	for (int y = 0; y < this->gridSize; y++)
	{
		for (int x = 0; x < this->gridSize; x++)
		{
			this->grid[x][y].baseCoord.x = x;
			this->grid[x][y].baseCoord.y = y;
			this->grid[x][y].screenCoord = ScreenSpaceConvert(this->grid[x][y].baseCoord);

			float xHeight = GetFunctionHeight(x, scalar, modifier, xPattern);
			float yHeight = GetFunctionHeight(y, scalar, modifier, yPattern);
			this->grid[x][y].height = (xHeight + yHeight) / 2;
			this->grid[x][y].screenCoord.y += this->grid[x][y].height;
			this->grid[x][y].centerPos.x = this->grid[x][y].screenCoord.x + this->tileSize / 2;
			this->grid[x][y].centerPos.y = this->grid[x][y].screenCoord.y + this->tileSize / 4;
		}
	}
	delete[] xPattern;
	delete[] yPattern;
}

*/