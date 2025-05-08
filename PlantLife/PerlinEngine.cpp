#include "PerlinEngine.h"
#include <iostream>
#pragma once 

PerlinEngine::PerlinEngine()
{
	table = new int[16];
	gradX = new float[16];
	gradY = new float[16];
	//Initialise grid,
	//By, assigning a random vector to each grid point
	for (int i = 0; i < 16; i++)
	{
		table[i] = i;
		gradX[i] = (float)std::rand() / (RAND_MAX / 2) - 1.0f;
		gradY[i] = (float)std::rand() / (RAND_MAX / 2) - 1.0f;
	}

	int j = 0;
	int swap = 0;
	for (int i = 0; i < 16; i++)
	{
		//Neat trick for capping the number at 16 - bitwise AND zeros any bits above the 8th place - cool!
		j = rand() & 15;

		//Fuddles around the numbers in the table to randomise it
		swap = table[i];
		table[i] = table[j];
		table[j] = swap;
	}
}

PerlinEngine::~PerlinEngine()
{
	delete table;
	delete gradX;
	delete gradY;
}