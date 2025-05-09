#include "PerlinEngine.h"
#include <iostream>
#pragma once 

PerlinEngine::PerlinEngine()
{
	table = new int[16];
	gradX = new float[16];
	gradY = new float[16];
	srand(time(0));
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

float PerlinEngine::Noise(float sampleX, float sampleY)
{
	//Gives the integer values for x and y of the grid intersecitons surrounding the sample point
	int x0 = int(floorf(sampleX));
	int x1 = x0 + 1;
	int y0 = int(floorf(sampleY));
	int y1 = y0 + 1;

	//Gets relative positions of sample within each grid square (as opposed to grid absolute - from 0,0).
	float relX0 = sampleX - float(x0);
	//Looks weird but gets the equivalent vector from the right hand side of the square
	float relX1 = relX0 - 1.0f;
	float relY0 = sampleY - float(y0);
	float relY1 = sampleY - 1.0f;

	//Getting dot products  between grid vectors and sample point
		//Gets the index from the table - this step unscrambles it. The question to me is why we scrambled it in the first place 
		//Ah ha, just a way of randomising properly for each run
		//The grid is actually just a long array which is why we're adding values together
	int gIndex = table[x0 + table[y0 & 15] & 15];
	//FOr dot-product at 0,0
	float d00 = (gradX[gIndex] * relX0) + (gradY[gIndex] * relY0);
	
	gIndex = table[x1 + table[y0 & 15] & 15];
	float d10 = (gradX[gIndex] * relX1) + (gradY[gIndex] * relY0);

	gIndex = table[x0 + table[y1 & 15] & 15];
	float d01 = (gradX[gIndex] * relX0) + (gradY[gIndex] * relY1);

	gIndex = table[x1 + table[y1 & 15] & 15];
	float d11 = (gradX[gIndex] * relX1) + (gradY[gIndex] * relY1);

	//Smoothing results using polynomial interpolation - we get a coefficient in each axis based on our sample point
	//Interesting way of typing 6X^5 - 15X^4+10X^3
	float coefX = ((6 * relX0 - 15) * relX0 + 10) * relX0 * relX0 * relX0; //With relX0 being the "vector from 0,0" in this case
	float coefY = ((6 * relY0 - 15) * relY0 + 10) * relY0 * relY0 * relY0;

	//Interpolation to get final value
	//DO the X=0 and X=1 dot products separately first to get am "X" dot product, whih can in turn be interpolated with the 
	float interpXA = d00 + coefX * (d01 - d00);
	float interpXB = d10 + coefX * (d11 - d00);
	return  interpXA + coefY * (interpXB - interpXA);

}