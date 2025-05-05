#include <iostream>
#include <SDL3/SDL.h>
#include "GardenState.h"
#pragma once

class WindowPainter
{
public:
	WindowPainter(int,int, int);
	~WindowPainter();

	SDL_Window* window;
	SDL_Renderer* wRenderer;
	SDL_Texture* tileTexture;
	SDL_Texture* splodge; //A splodge is like a splodge of point, used to draw plants

	
	
	bool PerformRenderCycle(GardenState*);

private:
	int screenResX;
	int screenResY;
	float leftLightCoef=0;
	float rightLightCoef=0;
	float topLightCoef = 0;
	int splodgeStartSize = 16;
	int circleRef16[16][16] =
	{
		
		{ 0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0 },
		{ 0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0 },
		{ 0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0 },
		{ 0,0,1,2,2,2,2,2,2,2,2,2,2,1,0,0 },
		{ 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
		{ 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
		{ 1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1 },
		{ 1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1 },
		{ 1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1 },
		{ 1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1 },
		{ 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
		{ 0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0 },
		{ 0,0,1,2,2,2,2,2,2,2,2,2,2,1,0,0 },
		{ 0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0 },
		{ 0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0 },
		{ 0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0 }
	};


	bool PaintGrid(GardenState*);
	bool RenderTileTexture(GardenState*);
	bool RenderSplodge();
	void GetLightingCoefficients(GardenState*);
	void RenderTestLine();
	void RenderBezier(SDL_Texture*, BezierSeg*,int, float, float);

};


/*


*/